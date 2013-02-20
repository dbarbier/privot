//                                               -*- C++ -*-
/**
 * @file  InverseBoxCoxEvaluationImplementation.cxx
 * @brief Class for the inverse Box cox function
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
 * @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */

#include "InverseBoxCoxEvaluationImplementation.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(InverseBoxCoxEvaluationImplementation);

static Factory<InverseBoxCoxEvaluationImplementation> RegisteredFactory("InverseBoxCoxEvaluationImplementation");

/* Default constructor */
InverseBoxCoxEvaluationImplementation::InverseBoxCoxEvaluationImplementation()
  : NumericalMathEvaluationImplementation()
{
  // Nothing to do
}

/* Parameter constructor */
InverseBoxCoxEvaluationImplementation::InverseBoxCoxEvaluationImplementation(const NumericalPoint & lambda)
  : NumericalMathEvaluationImplementation(),
    lambda_(lambda)
{
  // Nothing to do (maybe the description)
}

/* Clone constructor */
InverseBoxCoxEvaluationImplementation * InverseBoxCoxEvaluationImplementation::clone() const
{
  return new InverseBoxCoxEvaluationImplementation(*this);
}

/* Comparison operator */
Bool InverseBoxCoxEvaluationImplementation::operator ==(const InverseBoxCoxEvaluationImplementation & other) const
{
  return (lambda_ == other.lambda_);
}

/* String converter */
String InverseBoxCoxEvaluationImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << InverseBoxCoxEvaluationImplementation::GetClassName()
      << " name=" << getName()
      << " dimension = " << getInputDimension()
      << " lambda = " << lambda_.__repr__();
  return oss;
}

/* String converter __str__ */
String InverseBoxCoxEvaluationImplementation::__str__(const String & offset) const
{
  return OSS(false) << "lambda =" << lambda_.__str__(offset);
}

/* Accessor for the lambda */
NumericalPoint InverseBoxCoxEvaluationImplementation::getLambda() const
{
  return lambda_;
}

/* Operator () */
NumericalPoint InverseBoxCoxEvaluationImplementation::operator() (const NumericalPoint & inP) const
{
  if (inP.getDimension() != lambda_.getDimension()) throw InvalidArgumentException(HERE) << "Invalid input dimension";
  NumericalPoint result(lambda_.getDimension());

  // There is no check of positive variables
  // This last one must be done by user or, as the evaluation is used in a stochastic context, in the BoxCoxTransform class
  for (UnsignedLong index = 0; index < inP.getDimension(); ++index)
    {
      // Applying the Box-Cox function
      const NumericalScalar lambda_i(lambda_[index]);
      if (lambda_i == 0)
        result[index] = exp(inP[index]);
      else
        {
          const NumericalScalar evaluation(lambda_i * inP[index] + 1.0);
          if (evaluation <= 0) throw InvalidArgumentException(HERE) << "Can not apply the inverse Box Cox function " ;
          result[index] = pow((evaluation), 1.0 / lambda_i);
        }
    }
  ++callsNumber_;
  if (isHistoryEnabled_)
    {
      inputStrategy_.store(inP);
      outputStrategy_.store(result);
    }
  return result;
}

/* Accessor for input point dimension */
UnsignedLong InverseBoxCoxEvaluationImplementation::getInputDimension() const
{
  return lambda_.getDimension();
}

/* Accessor for output point dimension */
UnsignedLong InverseBoxCoxEvaluationImplementation::getOutputDimension() const
{
  return lambda_.getDimension();
}

/* Method save() stores the object through the StorageManager */
void InverseBoxCoxEvaluationImplementation::save(Advocate & adv) const
{
  NumericalMathEvaluationImplementation::save(adv);
  adv.saveAttribute( "lambda_", lambda_ );
}

/* Method load() reloads the object from the StorageManager */
void InverseBoxCoxEvaluationImplementation::load(Advocate & adv)
{
  NumericalMathEvaluationImplementation::load(adv);
  adv.loadAttribute( "lambda_", lambda_ );
}

END_NAMESPACE_OPENTURNS
