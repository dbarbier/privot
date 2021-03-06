//                                               -*- C++ -*-
/**
 * @file  BoxCoxEvaluationImplementation.cxx
 * @brief Class for a Box cox implementation
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

#include "BoxCoxEvaluationImplementation.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(BoxCoxEvaluationImplementation);

static Factory<BoxCoxEvaluationImplementation> RegisteredFactory("BoxCoxEvaluationImplementation");

/* Default constructor */
BoxCoxEvaluationImplementation::BoxCoxEvaluationImplementation()
  : NumericalMathEvaluationImplementation()
{
  // Nothing to do
}

/* Parameter constructor */
BoxCoxEvaluationImplementation::BoxCoxEvaluationImplementation(const NumericalPoint & lambda)
  : NumericalMathEvaluationImplementation(),
    lambda_(lambda)
{
  // Nothing to do (maybe the description)
}

/* Clone constructor */
BoxCoxEvaluationImplementation * BoxCoxEvaluationImplementation::clone() const
{
  return new BoxCoxEvaluationImplementation(*this);
}

/* Comparison operator */
Bool BoxCoxEvaluationImplementation::operator ==(const BoxCoxEvaluationImplementation & other) const
{
  return (lambda_ == other.lambda_);
}

/* String converter */
String BoxCoxEvaluationImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << BoxCoxEvaluationImplementation::GetClassName()
      << " name=" << getName()
      << " dimension = " << getInputDimension()
      << " lambda = " << lambda_.__repr__();
  return oss;
}

/* String converter __str__ */
String BoxCoxEvaluationImplementation::__str__(const String & offset) const
{
  OSS oss (false);
  oss << " lambda =" << lambda_.__str__(offset);
  return oss;
}

/* Accessor for the lambda */
NumericalPoint BoxCoxEvaluationImplementation::getLambda() const
{
  return lambda_;
}

/* Operator () */
NumericalPoint BoxCoxEvaluationImplementation::operator() (const NumericalPoint & inP) const
{
  if (inP.getDimension() != lambda_.getDimension()) throw InvalidArgumentException(HERE) << "Invalid input dimension";
  NumericalPoint result(lambda_.getDimension());

  // There is no check of positive variables
  // This last one must be done by user or, as the evaluation is used in a stochastic context, in the BoxCoxTransform class
  for (UnsignedLong index = 0; index < inP.getDimension(); ++index)
    {

      if (inP[index] <= 0)
        throw InvalidArgumentException(HERE) << "Can not apply the Box Cox evaluation function to negative values" ;

      // Applying the Box-Cox function
      const NumericalScalar lambda_i(lambda_[index]);
      if (lambda_i == 0)
        result[index] = log(inP[index]);
      else
        result[index] = ((pow(inP[index], lambda_i) - 1.0) / lambda_i);
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
UnsignedLong BoxCoxEvaluationImplementation::getInputDimension() const
{
  return lambda_.getDimension();
}

/* Accessor for output point dimension */
UnsignedLong BoxCoxEvaluationImplementation::getOutputDimension() const
{
  return lambda_.getDimension();
}

/* Method save() stores the object through the StorageManager */
void BoxCoxEvaluationImplementation::save(Advocate & adv) const
{
  NumericalMathEvaluationImplementation::save(adv);
  adv.saveAttribute( "lambda_", lambda_ );
}

/* Method load() reloads the object from the StorageManager */
void BoxCoxEvaluationImplementation::load(Advocate & adv)
{
  NumericalMathEvaluationImplementation::load(adv);
  adv.loadAttribute( "lambda_", lambda_ );
}

END_NAMESPACE_OPENTURNS
