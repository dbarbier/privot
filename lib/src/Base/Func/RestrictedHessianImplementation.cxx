//                                               -*- C++ -*-
/**
 * @file  RestrictedHessianImplementation.cxx
 * @brief The class that implements the marginal hessian of a given
 *        hessian, reference point and indices
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

#include "RestrictedHessianImplementation.hxx"
#include "NoNumericalMathHessianImplementation.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(RestrictedHessianImplementation);

static Factory<RestrictedHessianImplementation> RegisteredFactory("RestrictedHessianImplementation");


/* Default constructor */
RestrictedHessianImplementation::RestrictedHessianImplementation()
  : NumericalMathHessianImplementation()
  , p_hessian_(NoNumericalMathHessianImplementation().clone())
  , restrictionIndices_(0)
  , referencePoint_(0)
{
  // Nothing to do
} // RestrictedHessianImplementation

/* Default constructor */
RestrictedHessianImplementation::RestrictedHessianImplementation(const NumericalMathHessianImplementation & hessian,
                                                                 const Indices & restrictionIndices,
                                                                 const NumericalPoint & referencePoint)
  : NumericalMathHessianImplementation()
  , p_hessian_(hessian.clone())
  , restrictionIndices_(restrictionIndices)
  , referencePoint_(referencePoint)
{
  if (referencePoint.getDimension() != hessian.getInputDimension()) throw InvalidDimensionException(HERE) << "Error: the dimension of the reference point=" << referencePoint.getDimension() << " must match the input dimension of the hessian=" << hessian.getInputDimension();
  if (!restrictionIndices.check(referencePoint.getDimension() - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal function must be in the range [0, dim-1] and  must be different";
} // RestrictedHessianImplementation


/* Virtual constructor */
RestrictedHessianImplementation * RestrictedHessianImplementation::clone() const
{
  return new RestrictedHessianImplementation(*this);
}


/* Comparison operator */
Bool RestrictedHessianImplementation::operator ==(const RestrictedHessianImplementation & other) const
{
  return true;
}

/* String converter */
String RestrictedHessianImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << RestrictedHessianImplementation::GetClassName()
      << " name=" << getName()
      << " hessian=" << p_hessian_->__repr__()
      << " restrictionIndices=" << restrictionIndices_
      << " referencePoint=" << referencePoint_;
  return oss;
}

/* String converter */
String RestrictedHessianImplementation::__str__(const String & offset) const
{
  OSS oss(false);
  oss << offset;
  if (hasVisibleName()) oss << getName() << " :";
  oss << __repr__();
  return oss;
}

/* Hessian */
SymmetricTensor RestrictedHessianImplementation::hessian(const NumericalPoint & inP) const
{
  if (inP.getDimension() != getInputDimension()) throw InvalidArgumentException(HERE) << "Error: trying to evaluate a NumericalMathFunction with an argument of invalid dimension";
  NumericalPoint point(referencePoint_);
  const UnsignedLong inputDimension(getInputDimension());
  for (UnsignedLong i = 0; i < inputDimension; ++i) point[restrictionIndices_[i]] = inP[i];
  const SymmetricTensor fullHessian(p_hessian_->hessian(point));
  const UnsignedLong outputDimension(getOutputDimension());
  SymmetricTensor hessian(inputDimension, outputDimension);
  for (UnsignedLong i = 0; i < inputDimension; ++i)
    for (UnsignedLong j = 0; j <= i; ++j)
      for (UnsignedLong k = 0; k < outputDimension; ++k)
	hessian(i, j, k) = fullHessian(restrictionIndices_[i], restrictionIndices_[j], k);
  return hessian;
}

/* Accessor for input point dimension */
UnsignedLong RestrictedHessianImplementation::getInputDimension() const
{
  return restrictionIndices_.getSize();
}

/* Accessor for output point dimension */
UnsignedLong RestrictedHessianImplementation::getOutputDimension() const
{
  return p_hessian_->getOutputDimension();
}

/* Accessors to the hessian */
RestrictedHessianImplementation::Implementation RestrictedHessianImplementation::getHessian() const
{
  return p_hessian_;
}

void RestrictedHessianImplementation::setHessian(const NumericalMathHessianImplementation & hessian)
{
  if (referencePoint_.getDimension() != hessian.getInputDimension()) throw InvalidDimensionException(HERE) << "Error: the input dimension of the hessian=" << hessian.getInputDimension() << " must match the dimension of the reference point=" << referencePoint_.getDimension();
  p_hessian_ = hessian.clone();
}

/* Accessors to the restriction indices */
Indices RestrictedHessianImplementation::getRestrictionIndices() const
{
  return restrictionIndices_;
}

void RestrictedHessianImplementation::setRestrictionIndices(const Indices & restrictionIndices)
{
  if (!restrictionIndices.check(referencePoint_.getDimension() - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal function must be in the range [0, dim-1] and  must be different";
  restrictionIndices_ = restrictionIndices;
}

/* Accessors to the reference point */
NumericalPoint RestrictedHessianImplementation::getReferencePoint() const
{
  return referencePoint_;
}

void RestrictedHessianImplementation::setReferencePoint(const NumericalPoint & referencePoint)
{
  if (referencePoint.getDimension() != p_hessian_->getInputDimension()) throw InvalidDimensionException(HERE) << "Error: the dimension of the reference point=" << referencePoint.getDimension() << " must match the input dimension of the hessian=" << p_hessian_->getInputDimension();
  referencePoint_ = referencePoint;
}

/* Method save() stores the object through the StorageManager */
void RestrictedHessianImplementation::save(Advocate & adv) const
{
  NumericalMathHessianImplementation::save(adv);
  adv.saveAttribute( "hessian_", *p_hessian_ );
  adv.saveAttribute( "restrictionIndices_", restrictionIndices_ );
  adv.saveAttribute( "referencePoint_", referencePoint_ );
}

/* Method load() reloads the object from the StorageManager */
void RestrictedHessianImplementation::load(Advocate & adv)
{
  TypedInterfaceObject<NumericalMathHessianImplementation> hessianValue;
  NumericalMathHessianImplementation::load(adv);
  adv.loadAttribute( "hessian_", hessianValue );
  adv.loadAttribute( "restrictionIndices_", restrictionIndices_ );
  adv.loadAttribute( "referencePoint_", referencePoint_ );
  p_hessian_ = hessianValue.getImplementation();
}

END_NAMESPACE_OPENTURNS
