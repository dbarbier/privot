//                                               -*- C++ -*-
/**
 * @file  LinearNumericalMathEvaluationImplementation.cxx
 * @brief Class for a linear numerical math function implementation
 *        of the form y = constant + <linear, x - c> where c is a
 *        dim(x) numerical point, linear a dim(x) by dim(y) matrix
 *        and  <linear, x - c> means Transpose(linear).(x - c)
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

#include "LinearNumericalMathEvaluationImplementation.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(LinearNumericalMathEvaluationImplementation);

static Factory<LinearNumericalMathEvaluationImplementation> RegisteredFactory("LinearNumericalMathEvaluationImplementation");

/* Default constructor */
LinearNumericalMathEvaluationImplementation::LinearNumericalMathEvaluationImplementation()
  : NumericalMathEvaluationImplementation()
{
  // Nothing to do
}

/* Parameter constructor */
LinearNumericalMathEvaluationImplementation::LinearNumericalMathEvaluationImplementation(const NumericalPoint & center,
                                                                                         const NumericalPoint & constant,
                                                                                         const Matrix & linear)
  : NumericalMathEvaluationImplementation(),
    center_(center),
    constant_(constant),
    linear_(linear.transpose())
{
  /* Check if the dimension of the constant term is compatible with the linear term */
  if (constant.getDimension() != linear.getNbColumns()) throw InvalidDimensionException(HERE) << "Constant term dimension is incompatible with the linear term";
  /* Check if the dimension of the center term is compatible with the linear term */
  if (center.getDimension() != linear.getNbRows()) throw InvalidDimensionException(HERE) << "Center term dimension is incompatible with the linear term";
  Description description;
  for (UnsignedLong i = 0; i < center.getDimension(); i++)
    {
      OSS oss;
      oss << "x" << i;
      description.add(oss);
    }
  for (UnsignedLong i = 0; i < constant.getDimension(); i++)
    {
      OSS oss;
      oss << "y" << i;
      description.add(oss);
    }
  setDescription(description);
}

/* Virtual constructor */
LinearNumericalMathEvaluationImplementation * LinearNumericalMathEvaluationImplementation::clone() const
{
  return new LinearNumericalMathEvaluationImplementation(*this);
}

/* Comparison operator */
Bool LinearNumericalMathEvaluationImplementation::operator ==(const LinearNumericalMathEvaluationImplementation & other) const
{
  return ((linear_ == other.linear_) && (constant_ == other.constant_) && (center_ == other.center_));
}

/* String converter */
String LinearNumericalMathEvaluationImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << LinearNumericalMathEvaluationImplementation::GetClassName()
      << " name=" << getName()
      << " center=" << center_.__repr__()
      << " constant=" << constant_.__repr__()
      << " linear=" << linear_.transpose();
  return oss;
}

/* Accessor for the center */
NumericalPoint LinearNumericalMathEvaluationImplementation::getCenter() const
{
  return center_;
}

/* Accessor for the constant term */
NumericalPoint LinearNumericalMathEvaluationImplementation::getConstant() const
{
  return constant_;
}

/* Accessor for the linear term */
Matrix LinearNumericalMathEvaluationImplementation::getLinear() const
{
  return linear_.transpose();
}

/* Here is the interface that all derived class must implement */

/* Operator () */
NumericalPoint LinearNumericalMathEvaluationImplementation::operator() (const NumericalPoint & inP) const
{
  if (inP.getDimension() != center_.getDimension()) throw InvalidArgumentException(HERE) << "Invalid input dimension";
  const NumericalPoint result(constant_ + linear_ * (inP - center_));
  ++callsNumber_;
  if (isHistoryEnabled_)
    {
      inputStrategy_.store(inP);
      outputStrategy_.store(result);
    }
  return result;
}
/* Operator () */
NumericalSample LinearNumericalMathEvaluationImplementation::operator() (const NumericalSample & inS) const
{
  return NumericalMathEvaluationImplementation::operator()(inS);
}

/* Accessor for input point dimension */
UnsignedLong LinearNumericalMathEvaluationImplementation::getInputDimension() const
{
  return center_.getDimension();
}

/* Accessor for output point dimension */
UnsignedLong LinearNumericalMathEvaluationImplementation::getOutputDimension() const
{
  return constant_.getDimension();
}

/* Method save() stores the object through the StorageManager */
void LinearNumericalMathEvaluationImplementation::save(Advocate & adv) const
{
  NumericalMathEvaluationImplementation::save(adv);
  adv.saveAttribute( "center_", center_ );
  adv.saveAttribute( "constant_", constant_ );
  adv.saveAttribute( "linear_", linear_ );
}

/* Method load() reloads the object from the StorageManager */
void LinearNumericalMathEvaluationImplementation::load(Advocate & adv)
{
  NumericalMathEvaluationImplementation::load(adv);
  adv.loadAttribute( "center_", center_ );
  adv.loadAttribute( "constant_", constant_ );
  adv.loadAttribute( "linear_", linear_ );
}

END_NAMESPACE_OPENTURNS
