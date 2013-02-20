//                                               -*- C++ -*-
/**
 * @file  RestrictedGradientImplementation.cxx
 * @brief The class that implements the marginal gradient of a given
 *        gradient, reference point and indices
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

#include "RestrictedGradientImplementation.hxx"
#include "NoNumericalMathGradientImplementation.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(RestrictedGradientImplementation);

static Factory<RestrictedGradientImplementation> RegisteredFactory("RestrictedGradientImplementation");


/* Default constructor */
RestrictedGradientImplementation::RestrictedGradientImplementation()
  : NumericalMathGradientImplementation()
  , p_gradient_(NoNumericalMathGradientImplementation().clone())
  , restrictionIndices_(0)
  , referencePoint_(0)
{
  // Nothing to do
} // RestrictedGradientImplementation

/* Default constructor */
RestrictedGradientImplementation::RestrictedGradientImplementation(const NumericalMathGradientImplementation & gradient,
                                                                   const Indices & restrictionIndices,
                                                                   const NumericalPoint & referencePoint)
  : NumericalMathGradientImplementation()
  , p_gradient_(gradient.clone())
  , restrictionIndices_(restrictionIndices)
  , referencePoint_(referencePoint)
{
  if (referencePoint.getDimension() != gradient.getInputDimension()) throw InvalidDimensionException(HERE) << "Error: the dimension of the reference point=" << referencePoint.getDimension() << " must match the input dimension of the gradient=" << gradient.getInputDimension();
  if (!restrictionIndices.check(referencePoint.getDimension() - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal function must be in the range [0, dim-1] and  must be different";
} // RestrictedGradientImplementation


/* Virtual constructor */
RestrictedGradientImplementation * RestrictedGradientImplementation::clone() const
{
  return new RestrictedGradientImplementation(*this);
}


/* Comparison operator */
Bool RestrictedGradientImplementation::operator ==(const RestrictedGradientImplementation & other) const
{
  return true;
}

/* String converter */
String RestrictedGradientImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << RestrictedGradientImplementation::GetClassName()
      << " name=" << getName()
      << " gradient=" << p_gradient_->__repr__()
      << " restrictionIndices=" << restrictionIndices_
      << " referencePoint=" << referencePoint_;
  return oss;
}

/* String converter */
String RestrictedGradientImplementation::__str__(const String & offset) const
{
  OSS oss(false);
  oss << offset;
  if (hasVisibleName()) oss << getName() << " :";
  oss << __repr__();
  return oss;
}

/* Gradient */
Matrix RestrictedGradientImplementation::gradient(const NumericalPoint & inP) const
{
  if (inP.getDimension() != getInputDimension()) throw InvalidArgumentException(HERE) << "Error: trying to evaluate a NumericalMathFunction with an argument of invalid dimension";
  NumericalPoint point(referencePoint_);
  const UnsignedLong inputDimension(getInputDimension());
  for (UnsignedLong i = 0; i < inputDimension; ++i) point[restrictionIndices_[i]] = inP[i];
  const Matrix fullGradient(p_gradient_->gradient(point));
  const UnsignedLong outputDimension(getOutputDimension());
  Matrix gradient(inputDimension, outputDimension);
  for (UnsignedLong i = 0; i < inputDimension; ++i)
    for (UnsignedLong j = 0; j < outputDimension; ++j)
      gradient(i, j) = fullGradient(restrictionIndices_[i], j);
  return gradient;
}

/* Accessor for input point dimension */
UnsignedLong RestrictedGradientImplementation::getInputDimension() const
{
  return restrictionIndices_.getSize();
}

/* Accessor for output point dimension */
UnsignedLong RestrictedGradientImplementation::getOutputDimension() const
{
  return p_gradient_->getOutputDimension();
}

/* Accessors to the gradient */
RestrictedGradientImplementation::Implementation RestrictedGradientImplementation::getGradient() const
{
  return p_gradient_;
}

void RestrictedGradientImplementation::setGradient(const NumericalMathGradientImplementation & gradient)
{
  if (referencePoint_.getDimension() != gradient.getInputDimension()) throw InvalidDimensionException(HERE) << "Error: the input dimension of the gradient=" << gradient.getInputDimension() << " must match the dimension of the reference point=" << referencePoint_.getDimension();
  p_gradient_ = gradient.clone();
}

/* Accessors to the restriction indices */
Indices RestrictedGradientImplementation::getRestrictionIndices() const
{
  return restrictionIndices_;
}

void RestrictedGradientImplementation::setRestrictionIndices(const Indices & restrictionIndices)
{
  if (!restrictionIndices.check(referencePoint_.getDimension() - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal function must be in the range [0, dim-1] and  must be different";
  restrictionIndices_ = restrictionIndices;
}

/* Accessors to the reference point */
NumericalPoint RestrictedGradientImplementation::getReferencePoint() const
{
  return referencePoint_;
}

void RestrictedGradientImplementation::setReferencePoint(const NumericalPoint & referencePoint)
{
  if (referencePoint.getDimension() != p_gradient_->getInputDimension()) throw InvalidDimensionException(HERE) << "Error: the dimension of the reference point=" << referencePoint.getDimension() << " must match the input dimension of the gradient=" << p_gradient_->getInputDimension();
  referencePoint_ = referencePoint;
}

/* Method save() stores the object through the StorageManager */
void RestrictedGradientImplementation::save(Advocate & adv) const
{
  NumericalMathGradientImplementation::save(adv);
  adv.saveAttribute( "gradient_", *p_gradient_ );
  adv.saveAttribute( "restrictionIndices_", restrictionIndices_ );
  adv.saveAttribute( "referencePoint_", referencePoint_ );
}

/* Method load() reloads the object from the StorageManager */
void RestrictedGradientImplementation::load(Advocate & adv)
{
  TypedInterfaceObject<NumericalMathGradientImplementation> gradientValue;
  NumericalMathGradientImplementation::load(adv);
  adv.loadAttribute( "gradient_", gradientValue );
  adv.loadAttribute( "restrictionIndices_", restrictionIndices_ );
  adv.loadAttribute( "referencePoint_", referencePoint_ );
  p_gradient_ = gradientValue.getImplementation();
}

END_NAMESPACE_OPENTURNS
