//                                               -*- C++ -*-
/**
 * @file  AnalyticalNumericalMathGradientImplementation.cxx
 * @brief The class that implements the gradient of an analytical function.
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

#include "AnalyticalNumericalMathGradientImplementation.hxx"
#include "PersistentObjectFactory.hxx"
// From Ev3
#include "expression.h"
#include "parser.h"
#include "Log.hxx"
#include "Os.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(AnalyticalNumericalMathGradientImplementation);

static Factory<AnalyticalNumericalMathGradientImplementation> RegisteredFactory("AnalyticalNumericalMathGradientImplementation");

/* Default constructor */
AnalyticalNumericalMathGradientImplementation::AnalyticalNumericalMathGradientImplementation()
  : NumericalMathGradientImplementation(),
    isInitialized_(false),
    isAnalytical_(true),
    inputVariables_(0),
    evaluation_(),
    parsers_(0)
{
  // Nothing to do
} // AnalyticalNumericalMathGradientImplementation

/* Default constructor */
AnalyticalNumericalMathGradientImplementation::AnalyticalNumericalMathGradientImplementation(const AnalyticalNumericalMathEvaluationImplementation & evaluation)
  : NumericalMathGradientImplementation(),
    isInitialized_(false),
    isAnalytical_(true),
    inputVariables_(0),
    evaluation_(evaluation),
    parsers_(ParserCollection(0))
{
  // Nothing to do
} // AnalyticalNumericalMathGradientImplementation


/* Virtual constructor */
AnalyticalNumericalMathGradientImplementation * AnalyticalNumericalMathGradientImplementation::clone() const
{
  AnalyticalNumericalMathGradientImplementation * result = new AnalyticalNumericalMathGradientImplementation(*this);
  result->isInitialized_ = false;
  return result;
}


/* Comparison operator */
Bool AnalyticalNumericalMathGradientImplementation::operator ==(const AnalyticalNumericalMathGradientImplementation & other) const
{
  return (evaluation_ == other.evaluation_);
}

/* String converter */
String AnalyticalNumericalMathGradientImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << AnalyticalNumericalMathGradientImplementation::GetClassName()
      << " name=" << getName()
      << " evaluation=" << evaluation_;
  return oss;
}

/* String converter */
String AnalyticalNumericalMathGradientImplementation::__str__(const String & offset) const
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
          for (UnsignedLong i = 0; i < getInputDimension(); ++i)
            {
              const UnsignedLong lI(inputVariablesNames[i].length());
              for (UnsignedLong j = 0; j < getOutputDimension(); ++j)
                {
                  const UnsignedLong l(lI + outputVariablesNames[j].length() + 8);
                  if (l > length) length = l;
                }
            }
          for (UnsignedLong j = 0; j < getOutputDimension(); ++j)
            {
              for (UnsignedLong i = 0; i < getInputDimension(); ++i)
                {
                  oss << offset << "| " << std::setw(length) << ("d(" + outputVariablesNames[j] + ") / d(" + inputVariablesNames[i] + ")") << " = " << getFormula(i, j) << Os::GetEndOfLine();
                }
            }
        } // isAnalytical
    }
  catch(...)
    {
      // Nothing to do
    }
  if (!isAnalytical_) oss << offset << "No analytical gradient available. Try using finite difference instead.";
  return oss;
}

/* Must initialize the parser at the first call to operator() as the
   reference associated with the variables may have change after the construction */
void AnalyticalNumericalMathGradientImplementation::initialize() const
{
  if (isInitialized_) return;
  try
    {
      isAnalytical_ = false;
      const UnsignedLong inputSize(evaluation_.inputVariablesNames_.getSize());
      const UnsignedLong outputSize(evaluation_.outputVariablesNames_.getSize());
      const UnsignedLong gradientSize(inputSize * outputSize);
      parsers_ = ParserCollection(gradientSize);
      inputVariables_ = NumericalScalarCollection(inputSize);
      // For each element of the gradient, do
      UnsignedLong gradientIndex(0);
      for (UnsignedLong columnIndex = 0; columnIndex < outputSize; ++columnIndex)
        {
          // Parse the current formula with Ev3
          int nerr(0);
          Ev3::ExpressionParser ev3Parser;
          // Initialize the variable indices in order to match the order of OpenTURNS in Ev3
          for (UnsignedLong inputVariableIndex = 0; inputVariableIndex < inputSize; ++inputVariableIndex) ev3Parser.SetVariableID(evaluation_.inputVariablesNames_[inputVariableIndex], inputVariableIndex);
          Ev3::Expression ev3Expression(ev3Parser.Parse(evaluation_.formulas_[columnIndex].c_str(), nerr));
          if (nerr != 0) throw InvalidArgumentException(HERE) << "Error: cannot parse " << evaluation_.formulas_[columnIndex] << " with Ev3. No analytical gradient.";
          //                Ev3::Simplify(&ev3Expression);
          for (UnsignedLong rowIndex = 0; rowIndex < inputSize; ++rowIndex)
            {
              // First, define all the variable names and associate them
              // to the corresponding component of the input vector
              for(UnsignedLong inputVariableIndex = 0; inputVariableIndex < inputSize; ++inputVariableIndex) parsers_[gradientIndex].DefineVar(evaluation_.inputVariablesNames_[inputVariableIndex].c_str(), &inputVariables_[inputVariableIndex]);
              // Second, define the formula
              try
                {
                  Ev3::Expression derivative(Ev3::Diff(ev3Expression, rowIndex));
                  //                    Ev3::Simplify(&derivative);
                  LOGINFO(OSS() << "d(" << ev3Expression->ToString() << ")/d(" << evaluation_.inputVariablesNames_[rowIndex] << ")=" << derivative->ToString());
                  parsers_[gradientIndex].SetExpr(derivative->ToString().c_str());
                  ++gradientIndex;
                }
              catch(...)
                {
                  throw InternalException(HERE) << "Error: cannot compute the derivative of " << ev3Expression->ToString() << " with respect to " << evaluation_.inputVariablesNames_[rowIndex];
                }
            }
        }
    }
  catch(mu::Parser::exception_type & ex)
    {
      // Here, we know that both isAnalytical_ and isInitialized_ are false
      throw InvalidArgumentException(HERE) << "Error constructing the gradient of an analytical function, message=" << ex.GetMsg() << " formula=" << ex.GetExpr() << " token=" << ex.GetToken() << " position=" << ex.GetPos();
    }
  // Everything is ok (no exception)
  isAnalytical_ = true;
  isInitialized_ = true;
}

/* Gradient */
Matrix AnalyticalNumericalMathGradientImplementation::gradient(const NumericalPoint & inP) const
{
  const UnsignedLong inputDimension(getInputDimension());
  if (inP.getDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: trying to evaluate a NumericalMathFunction with an argument of invalid dimension";
  if (!isInitialized_) initialize();
  if (!isAnalytical_) throw InternalException(HERE) << "The gradient does not have an analytical expression.";
  for (UnsignedLong i = 0; i < inP.getDimension(); ++i) inputVariables_[i] = inP[i];
  const UnsignedLong outputDimension(getOutputDimension());
  Matrix out(inputDimension, outputDimension);
  ++callsNumber_;
  try
    {
      UnsignedLong parserIndex(0);
      for (UnsignedLong columnIndex = 0; columnIndex < outputDimension; ++columnIndex)
        {
          for (UnsignedLong rowIndex = 0; rowIndex < inputDimension; ++rowIndex)
            {
              out(rowIndex, columnIndex) = parsers_[parserIndex].Eval();
              ++parserIndex;
            }
        }
    }
  catch(mu::Parser::exception_type & ex)
    {
      throw InternalException(HERE) << ex.GetMsg();
    }
  return out;
}

/* Accessor for input point dimension */
UnsignedLong AnalyticalNumericalMathGradientImplementation::getInputDimension() const
{
  return evaluation_.getInputDimension();
}

/* Accessor for output point dimension */
UnsignedLong AnalyticalNumericalMathGradientImplementation::getOutputDimension() const
{
  return evaluation_.getOutputDimension();
}

/* Accessor to a specific formula */
String AnalyticalNumericalMathGradientImplementation::getFormula(const UnsignedLong i,
                                                                 const UnsignedLong j) const
{
  const UnsignedLong inputDimension(getInputDimension());
  if ((i >= inputDimension) || (j >= getOutputDimension())) throw InvalidArgumentException(HERE) << "Error: cannot access to a formula outside of the gradient dimensions.";
  if (!isInitialized_) initialize();
  return parsers_[i + j * inputDimension].GetExpr();
}

/* Get the i-th marginal function */
AnalyticalNumericalMathGradientImplementation::Implementation AnalyticalNumericalMathGradientImplementation::getMarginal(const UnsignedLong i) const
{
  if (i >= getOutputDimension()) throw InvalidArgumentException(HERE) << "Error: the index of a marginal gradient must be in the range [0, outputDimension-1]";
  return getMarginal(Indices(1, i));
}

/* Get the function corresponding to indices components */
AnalyticalNumericalMathGradientImplementation::Implementation AnalyticalNumericalMathGradientImplementation::getMarginal(const Indices & indices) const
{
  if (!indices.check(getOutputDimension() - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal gradient must be in the range [0, dim-1] and  must be different";
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
  return new AnalyticalNumericalMathGradientImplementation(AnalyticalNumericalMathEvaluationImplementation(evaluation_.getInputVariablesNames(), marginalOutputNames, marginalFormulas));
}

/* Method save() stores the object through the StorageManager */
void AnalyticalNumericalMathGradientImplementation::save(Advocate & adv) const
{
  NumericalMathGradientImplementation::save(adv);
  adv.saveAttribute( "evaluation_", evaluation_ );
}

/* Method load() reloads the object from the StorageManager */
void AnalyticalNumericalMathGradientImplementation::load(Advocate & adv)
{
  NumericalMathGradientImplementation::load(adv);
  adv.loadAttribute( "evaluation_", evaluation_ );
  *this = AnalyticalNumericalMathGradientImplementation(evaluation_);
}

END_NAMESPACE_OPENTURNS
