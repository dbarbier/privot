//                                               -*- C++ -*-
/**
 * @file  AnalyticalNumericalMathEvaluationImplementation.cxx
 * @brief The class that implements the evaluation of an analytical function.
 *
 * Copyright (C) 2005-2013 EDF-EADS-Phimeca
 *
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 *
 *
 * @author schueller
 * @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */
#include <iomanip>

#include "AnalyticalNumericalMathEvaluationImplementation.hxx"
#include "PersistentObjectFactory.hxx"
#include "Os.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(AnalyticalNumericalMathEvaluationImplementation);

static Factory<AnalyticalNumericalMathEvaluationImplementation> RegisteredFactory("AnalyticalNumericalMathEvaluationImplementation");


/* Default constructor */
AnalyticalNumericalMathEvaluationImplementation::AnalyticalNumericalMathEvaluationImplementation()
  : NumericalMathEvaluationImplementation(),
    isInitialized_(false),
    inputVariables_(0),
    inputVariablesNames_(),
    outputVariablesNames_(),
    formulas_(),
    parsers_(0)
{
  // Nothing to do
} // AnalyticalNumericalMathEvaluationImplementation

/* Default constructor */
AnalyticalNumericalMathEvaluationImplementation::AnalyticalNumericalMathEvaluationImplementation(const Description & inputVariablesNames,
                                                                                                 const Description & outputVariablesNames,
                                                                                                 const Description & formulas)
  : NumericalMathEvaluationImplementation(),
    isInitialized_(false),
    inputVariables_(NumericalPoint(inputVariablesNames.getSize()).getCollection()),
    inputVariablesNames_(inputVariablesNames),
    outputVariablesNames_(outputVariablesNames),
    formulas_(formulas),
    parsers_(ParserCollection(outputVariablesNames_.getSize()))
{
  if (outputVariablesNames.getSize() != formulas.getSize())
    throw InvalidDimensionException(HERE) << "The number of outputVariablesNames (" << outputVariablesNames.getSize()
                                          << ") does not match the number of formulas (" << formulas.getSize() << ")";

  // Call the initialize() method in order to check the parameters w.r.t. muParser
  initialize();
  Description description(inputVariablesNames_);
  const UnsignedLong outputSize(outputVariablesNames_.getSize());
  for (UnsignedLong outputVariableIndex = 0; outputVariableIndex < outputSize; ++outputVariableIndex)
    description.add(outputVariablesNames_[outputVariableIndex]);
  // Set the description
  setDescription(description);
} // AnalyticalNumericalMathEvaluationImplementation


/* Virtual constructor */
AnalyticalNumericalMathEvaluationImplementation * AnalyticalNumericalMathEvaluationImplementation::clone() const
{
  AnalyticalNumericalMathEvaluationImplementation * result = new AnalyticalNumericalMathEvaluationImplementation(*this);
  result->isInitialized_ = false;
  return result;
}


/* Comparison operator */
Bool AnalyticalNumericalMathEvaluationImplementation::operator ==(const AnalyticalNumericalMathEvaluationImplementation & other) const
{
  return true;
}

/* String converter */
String AnalyticalNumericalMathEvaluationImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << AnalyticalNumericalMathEvaluationImplementation::GetClassName()
      << " name=" << getName()
      << " inputVariablesNames=" << inputVariablesNames_
      << " outputVariablesNames=" << outputVariablesNames_
      << " formulas=" << formulas_;
  return oss;
}

/* String converter */
String AnalyticalNumericalMathEvaluationImplementation::__str__(const String & offset) const
{
  OSS oss(false);
  oss << offset;
  if (hasVisibleName()) oss << getName() << " :";
  const UnsignedLong outputDimension(getOutputDimension());
  if (getOutputDimension() > 1)
    {
      oss << "\n";
      // First, find the maximum length of the output variable names
      UnsignedLong length(0);
      for (UnsignedLong i = 0; i < getOutputDimension(); ++i)
        {
          const UnsignedLong l(outputVariablesNames_[i].length());
          if (l > length) length = l;
        }
      oss << "\n";
      for (UnsignedLong i = 0; i < getOutputDimension(); ++i)
        {
          oss << offset << "| " << std::setw(length) << outputVariablesNames_[i] << " = " << formulas_[i] << "\n";
        }
    } // decorated
  else
    {
      oss << formulas_[0];
      for (UnsignedLong i = 1; i < outputDimension; ++i) oss << ", " << formulas_[i];
    }
  return oss;
}

/* Must initialize the parser at the first call to operator() as the
   reference associated with the variables may have change after the construction */
void AnalyticalNumericalMathEvaluationImplementation::initialize() const
{
  if (isInitialized_) return;
  const UnsignedLong inputSize(inputVariablesNames_.getSize());
  const UnsignedLong outputSize(outputVariablesNames_.getSize());
  try
    {
      // For each parser of a formula, do
      for (UnsignedLong outputVariableIndex = 0; outputVariableIndex < outputSize; ++outputVariableIndex)
        {
          // First, define all the variable names and associate them
          // to the corresponding component of the input vector
          for(UnsignedLong inputVariableIndex = 0; inputVariableIndex < inputSize; ++inputVariableIndex)
            {
              parsers_[outputVariableIndex].DefineVar(inputVariablesNames_[inputVariableIndex].c_str(), &inputVariables_[inputVariableIndex]);
            }
          // Second, define the several formulas
          parsers_[outputVariableIndex].SetExpr(formulas_[outputVariableIndex].c_str());
        }
    }
  catch(mu::Parser::exception_type & ex)
    {
      throw InvalidArgumentException(HERE) << "Error constructing an analytical function, message=" << ex.GetMsg() << " formula=" << ex.GetExpr() << " token=" << ex.GetToken() << " position=" << ex.GetPos();
    }
  isInitialized_ = true;
}

/* Operator () */
NumericalPoint AnalyticalNumericalMathEvaluationImplementation::operator() (const NumericalPoint & inP) const
{
  if (inP.getDimension() != getInputDimension()) throw InvalidArgumentException(HERE) << "Error: trying to evaluate a NumericalMathFunction with an argument of invalid dimension";
  if (!isInitialized_) initialize();
  NumericalPoint result(getOutputDimension());
  ++callsNumber_;
  for (UnsignedLong i = 0; i < inP.getDimension(); ++i) inputVariables_[i] = inP[i];
  try
    {
      for (UnsignedLong index = 0; index < result.getDimension(); ++index) result[index] = parsers_[index].Eval();
    }
  catch(mu::Parser::exception_type & ex)
    {
      throw InternalException(HERE) << ex.GetMsg();
    }
  if (isHistoryEnabled_)
    {
      inputStrategy_.store(inP);
      outputStrategy_.store(result);
    }
  return result;
}

/* Accessor for input point dimension */
UnsignedLong AnalyticalNumericalMathEvaluationImplementation::getInputDimension() const
{
  return inputVariablesNames_.getSize();
}

/* Accessor for output point dimension */
UnsignedLong AnalyticalNumericalMathEvaluationImplementation::getOutputDimension() const
{
  return outputVariablesNames_.getSize();
}

/* Get the i-th marginal function */
AnalyticalNumericalMathEvaluationImplementation::Implementation AnalyticalNumericalMathEvaluationImplementation::getMarginal(const UnsignedLong i) const
{
  if (i >= getOutputDimension()) throw InvalidArgumentException(HERE) << "Error: the index of a marginal function must be in the range [0, outputDimension-1]";
  return new AnalyticalNumericalMathEvaluationImplementation(inputVariablesNames_, Description(1, outputVariablesNames_[i]), Description(1, formulas_[i]));
}

/* Get the function corresponding to indices components */
AnalyticalNumericalMathEvaluationImplementation::Implementation AnalyticalNumericalMathEvaluationImplementation::getMarginal(const Indices & indices) const
{
  if (!indices.check(getOutputDimension() - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal function must be in the range [0, dim-1] and  must be different";
  const UnsignedLong size(indices.getSize());
  Description marginalOutputVariablesNames(size);
  Description marginalFormulas(size);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const UnsignedLong j(indices[i]);
      marginalOutputVariablesNames[i] = outputVariablesNames_[j];
      marginalFormulas[i] = formulas_[j];
    }
  return new AnalyticalNumericalMathEvaluationImplementation(inputVariablesNames_, marginalOutputVariablesNames, marginalFormulas);
}

/* Accessor to the input variables names */
Description AnalyticalNumericalMathEvaluationImplementation::getInputVariablesNames() const
{
  return inputVariablesNames_;
}

/* Accessor to the output variables names */
Description AnalyticalNumericalMathEvaluationImplementation::getOutputVariablesNames() const
{
  return outputVariablesNames_;
}

/* Accessor to the formulas */
Description AnalyticalNumericalMathEvaluationImplementation::getFormulas() const
{
  return formulas_;
}

/* Method save() stores the object through the StorageManager */
void AnalyticalNumericalMathEvaluationImplementation::save(Advocate & adv) const
{
  NumericalMathEvaluationImplementation::save(adv);
  adv.saveAttribute( "inputVariablesNames_", inputVariablesNames_ );
  adv.saveAttribute( "outputVariablesNames_", outputVariablesNames_ );
  adv.saveAttribute( "formulas_", formulas_ );
}

/* Method load() reloads the object from the StorageManager */
void AnalyticalNumericalMathEvaluationImplementation::load(Advocate & adv)
{
  NumericalMathEvaluationImplementation::load(adv);
  adv.loadAttribute( "inputVariablesNames_", inputVariablesNames_ );
  adv.loadAttribute( "outputVariablesNames_", outputVariablesNames_ );
  adv.loadAttribute( "formulas_", formulas_ );
  *this = AnalyticalNumericalMathEvaluationImplementation(inputVariablesNames_, outputVariablesNames_, formulas_);
}

END_NAMESPACE_OPENTURNS
