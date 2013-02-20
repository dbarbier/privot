//                                               -*- C++ -*-
/**
 * @file  AnalyticalNumericalMathHessianImplementation.cxx
 * @brief The class that implements the Hessian of analytical functions.
 *
 * Copyright (C) 2005-2013 EDF-EADS-Phimeca
 *
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 *
 *
 * @author lebrun
 * @date   2010-05-31 07:26:50 +0200 (lun. 31 mai 2010)
 */

#include "AnalyticalNumericalMathHessianImplementation.hxx"
#include "PersistentObjectFactory.hxx"
// From Ev3
#include "expression.h"
#include "parser.h"
#include "Log.hxx"
#include "Exception.hxx"
#include "Os.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(AnalyticalNumericalMathHessianImplementation);

static Factory<AnalyticalNumericalMathHessianImplementation> RegisteredFactory("AnalyticalNumericalMathHessianImplementation");


/* Default constructor */
AnalyticalNumericalMathHessianImplementation::AnalyticalNumericalMathHessianImplementation()
  : NumericalMathHessianImplementation(),
    isInitialized_(false),
    isAnalytical_(true),
    inputVariables_(0),
    evaluation_(),
    parsers_(0)
{
  // Nothing to do
} // AnalyticalNumericalMathHessianImplementation

/* Default constructor */
AnalyticalNumericalMathHessianImplementation::AnalyticalNumericalMathHessianImplementation(const AnalyticalNumericalMathEvaluationImplementation & evaluation)
  : NumericalMathHessianImplementation(),
    isInitialized_(false),
    isAnalytical_(true),
    inputVariables_(0),
    evaluation_(evaluation),
    parsers_(ParserCollection(0))
{
  // Nothing to do
} // AnalyticalNumericalMathHessianImplementation


/* Virtual constructor */
AnalyticalNumericalMathHessianImplementation * AnalyticalNumericalMathHessianImplementation::clone() const
{
  AnalyticalNumericalMathHessianImplementation * result = new AnalyticalNumericalMathHessianImplementation(*this);
  result->isInitialized_ = false;
  return result;
}


/* Comparison operator */
Bool AnalyticalNumericalMathHessianImplementation::operator ==(const AnalyticalNumericalMathHessianImplementation & other) const
{
  return true;
}

/* String converter */
String AnalyticalNumericalMathHessianImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << AnalyticalNumericalMathHessianImplementation::GetClassName()
      << " name=" << getName()
      << " evaluation=" << evaluation_;
  return oss;
}

/* String converter */
String AnalyticalNumericalMathHessianImplementation::__str__(const String & offset) const
{
  OSS oss(false);
  oss << offset;
  if (hasVisibleName()) oss << getName() << " :";
  oss << "\n";
  // Check that the symbolic differentiation has been done
  try
    {
      initialize();
      if (isAnalytical_)
        {
          // First, find the maximum length of the output variable names
          const Description inputVariablesNames(evaluation_.getInputVariablesNames());
          const Description outputVariablesNames(evaluation_.getOutputVariablesNames());
          UnsignedLong length(0);
          for (UnsignedLong k = 0; k < getOutputDimension(); ++k)
            {
              const UnsignedLong lK(outputVariablesNames[k].length());
              for (UnsignedLong i = 0; i < getInputDimension(); ++i)
                {
                  const UnsignedLong lI(inputVariablesNames[i].length());
                  // The diagonal term is always shorter than one of the off-diagonal terms
                  for (UnsignedLong j = 0; j < i; ++j)
                    {
                      const UnsignedLong l(lI + lK + inputVariablesNames[k].length() + 14);
                      if (l > length) length = l;
                    }
                } // For i
            } // For k
          for (UnsignedLong k = 0; k < getOutputDimension(); ++k)
            {
              for (UnsignedLong i = 0; i < getInputDimension(); ++i)
                {
                  for (UnsignedLong j = 0; j < i; ++j)
                    {
                      oss << offset << "| " << std::setw(length) << ("d^2(" + outputVariablesNames[k] + ") / d(" + inputVariablesNames[i] + ")d(" + inputVariablesNames[j] + ")") << " = " << getFormula(i, j, k) << Os::GetEndOfLine();
                    }
                  oss << offset << "| " << std::setw(length) << ("d^2(" + outputVariablesNames[k] + ") / d(" + inputVariablesNames[i] + ")^2") << " = " << getFormula(i, i, k) << Os::GetEndOfLine();
                } // For i
            } // For k
        } // isAnalytical
    }
  catch(...)
    {
      // Nothing to do
    }
  if (!isAnalytical_) oss << offset << "No analytical hessian available. Try using finite difference instead.";
  return oss;
}

/* Must initialize the parser at the first call to operator() as the
   reference associated with the variables may have change after the construction */
void AnalyticalNumericalMathHessianImplementation::initialize() const
{
  if (isInitialized_) return;
  try
    {
      isAnalytical_ = false;
      const UnsignedLong inputSize(evaluation_.inputVariablesNames_.getSize());
      const UnsignedLong outputSize(evaluation_.outputVariablesNames_.getSize());
      const UnsignedLong hessianSize(inputSize * (inputSize + 1) * outputSize / 2);
      parsers_ = ParserCollection(hessianSize);
      inputVariables_ = NumericalScalarCollection(inputSize);
      // For each element of the hessian, do
      UnsignedLong hessianIndex(0);
      for (UnsignedLong sheetIndex = 0; sheetIndex < outputSize; ++sheetIndex)
        {
          // Parse the current formula with Ev3
          int nerr(0);
          Ev3::ExpressionParser ev3Parser;
          // Initialize the variable indices in order to match the order of OpenTURNS in Ev3
          for (UnsignedLong inputVariableIndex = 0; inputVariableIndex < inputSize; ++inputVariableIndex) ev3Parser.SetVariableID(evaluation_.inputVariablesNames_[inputVariableIndex], inputVariableIndex);
          Ev3::Expression ev3Expression = ev3Parser.Parse(evaluation_.formulas_[sheetIndex].c_str(), nerr);
          if (nerr != 0) throw InvalidArgumentException(HERE) << "Error: cannot parse " << evaluation_.formulas_[sheetIndex] << " with Ev3. No analytical hessian.";
          //                Ev3::Simplify(&ev3Expression);
          for (UnsignedLong rowIndex = 0; rowIndex < inputSize; ++rowIndex)
            {
              Ev3::Expression firstDerivative;
              try
                {
                  firstDerivative = Ev3::Diff(ev3Expression, rowIndex);
                  //                    Ev3::Simplify(&firstDerivative);
                  LOGINFO(OSS() << "First variable=" << evaluation_.inputVariablesNames_[rowIndex] << ", derivative=" << firstDerivative->ToString());
                }
              catch(...)
                {
                  throw InternalException(HERE) << "Error: cannot compute the derivative of " << ev3Expression->ToString() << " with respect to " << evaluation_.inputVariablesNames_[rowIndex];
                }
              for (UnsignedLong columnIndex = 0; columnIndex <= rowIndex; ++columnIndex)
                {
                  // First, define all the variable names and associate them
                  // to the corresponding component of the input vector
                  for(UnsignedLong inputVariableIndex = 0; inputVariableIndex < inputSize; ++inputVariableIndex) parsers_[hessianIndex].DefineVar(evaluation_.inputVariablesNames_[inputVariableIndex].c_str(), &inputVariables_[inputVariableIndex]);
                  // Second, define the formula
                  try
                    {
                      Ev3::Expression secondDerivative(Ev3::Diff(firstDerivative, columnIndex));
                      //                        Ev3::Simplify(&secondDerivative);
                      LOGINFO(OSS() << "d2(" << ev3Expression->ToString() << ")/d(" << evaluation_.inputVariablesNames_[rowIndex] << ")d(" << evaluation_.inputVariablesNames_[columnIndex] << ")=" << secondDerivative->ToString());
                      parsers_[hessianIndex].SetExpr(secondDerivative->ToString().c_str());
                      ++hessianIndex;
                    }
                  catch(...)
                    {
                      throw InternalException(HERE) << "Error: cannot compute the derivative of " << firstDerivative->ToString() << " with respect to " << evaluation_.inputVariablesNames_[columnIndex];
                    }
                } // columnIndex
            } // rowIndex
        } // sheetIndex
    }
  catch(mu::Parser::exception_type & ex)
    {
      // Here, we know that both isAnalytical_ and isInitialized_ are false
      throw InvalidArgumentException(HERE) << "Error constructing the hessian of an analytical function, message=" << ex.GetMsg() << " formula=" << ex.GetExpr() << " token=" << ex.GetToken() << " position=" << ex.GetPos();
    }
  // Everything is ok (no exception)
  isAnalytical_ = true;
  isInitialized_ = true;
}

/* Hessian */
SymmetricTensor AnalyticalNumericalMathHessianImplementation::hessian(const NumericalPoint & inP) const
{
  const UnsignedLong inputDimension(getInputDimension());
  if (inP.getDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: trying to evaluate a NumericalMathFunction with an argument of invalid dimension";
  if (!isInitialized_) initialize();
  if (!isAnalytical_) throw InternalException(HERE) << "The hessian does not have an analytical expression.";
  for (UnsignedLong i = 0; i < inP.getDimension(); ++i) inputVariables_[i] = inP[i];
  const UnsignedLong outputDimension(getOutputDimension());
  SymmetricTensor out(inputDimension, outputDimension);
  ++callsNumber_;
  try
    {
      UnsignedLong parserIndex(0);
      for (UnsignedLong sheetIndex = 0; sheetIndex < outputDimension; ++sheetIndex)
        {
          for (UnsignedLong rowIndex = 0; rowIndex < inputDimension; ++rowIndex)
            {
              for (UnsignedLong columnIndex = 0; columnIndex <= rowIndex; ++columnIndex)
                {
                  out(rowIndex, columnIndex, sheetIndex) = parsers_[parserIndex].Eval();
                  ++parserIndex;
                } // columnIndex
            } // rowIndex
        } // sheetIndex
    }
  catch(mu::Parser::exception_type & ex)
    {
      throw InternalException(HERE) << ex.GetMsg();
    }
  return out;
}

/* Accessor to a specific formula */
String AnalyticalNumericalMathHessianImplementation::getFormula(const UnsignedLong i,
                                                                const UnsignedLong j,
                                                                const UnsignedLong k) const
{
  const UnsignedLong inputDimension(getInputDimension());
  if ((i >= inputDimension) || (j >= inputDimension) || (k >= getOutputDimension())) throw InvalidArgumentException(HERE) << "Error: cannot access to a formula outside of the hessian dimensions.";
  if (!isInitialized_) initialize();
  // Convert the 3D index into a linear index
  UnsignedLong rowIndex(i);
  UnsignedLong columnIndex(j);
  UnsignedLong sheetIndex(k);
  // First, take the symmetry into account
  if (i < j)
    {
      rowIndex = j;
      columnIndex = i;
    }
  // Now, columnIndex <= rowIndex
  UnsignedLong linearIndex(0);
  // Each sheet adds a triangle with the main diagonal
  linearIndex += ((inputDimension * (inputDimension + 1)) / 2) * sheetIndex;
  // Compute the linear sub-index into the triangle
  linearIndex += (rowIndex * (rowIndex + 1)) / 2 + columnIndex;
  return parsers_[linearIndex].GetExpr();
}

/* Accessor for input point dimension */
UnsignedLong AnalyticalNumericalMathHessianImplementation::getInputDimension() const
{
  return evaluation_.getInputDimension();
}

/* Accessor for output point dimension */
UnsignedLong AnalyticalNumericalMathHessianImplementation::getOutputDimension() const
{
  return evaluation_.getOutputDimension();
}

/* Get the i-th marginal function */
AnalyticalNumericalMathHessianImplementation::Implementation AnalyticalNumericalMathHessianImplementation::getMarginal(const UnsignedLong i) const
{
  if (i >= getOutputDimension()) throw InvalidArgumentException(HERE) << "Error: the index of a marginal hessian must be in the range [0, outputDimension-1]";
  return getMarginal(Indices(1, i));
}

/* Get the function corresponding to indices components */
AnalyticalNumericalMathHessianImplementation::Implementation AnalyticalNumericalMathHessianImplementation::getMarginal(const Indices & indices) const
{
  if (!indices.check(getOutputDimension() - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal hessian must be in the range [0, dim-1] and  must be different";
  const UnsignedLong marginalDimension(indices.getSize());
  Description marginalFormulas(marginalDimension);
  Description marginalOutputNames(marginalDimension);
  Description outputNames(evaluation_.getOutputVariablesNames());
  Description formulas(evaluation_.getFormulas());
  for (UnsignedLong i = 0; i < marginalDimension; ++i)
    {
      marginalFormulas[i] = formulas[indices[i]];
      marginalOutputNames[i] = outputNames[indices[i]];
    }
  return new AnalyticalNumericalMathHessianImplementation(AnalyticalNumericalMathEvaluationImplementation(evaluation_.getInputVariablesNames(), marginalOutputNames, marginalFormulas));
}

/* Method save() stores the object through the StorageManager */
void AnalyticalNumericalMathHessianImplementation::save(Advocate & adv) const
{
  NumericalMathHessianImplementation::save(adv);
  adv.saveAttribute( "evaluation_", evaluation_ );
}

/* Method load() reloads the object from the StorageManager */
void AnalyticalNumericalMathHessianImplementation::load(Advocate & adv)
{
  NumericalMathHessianImplementation::load(adv);
  adv.loadAttribute( "evaluation_", evaluation_ );
  *this = AnalyticalNumericalMathHessianImplementation(evaluation_);
}

END_NAMESPACE_OPENTURNS
