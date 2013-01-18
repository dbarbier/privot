//                                               -*- C++ -*-
/**
 * @file  RestrictedEvaluationImplementation.cxx
 * @brief The class that implements the marginal evaluation of a given
 *        evaluation, reference point and indices
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

#include "RestrictedEvaluationImplementation.hxx"
#include "NoNumericalMathEvaluationImplementation.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(RestrictedEvaluationImplementation);

static Factory<RestrictedEvaluationImplementation> RegisteredFactory("RestrictedEvaluationImplementation");


/* Default constructor */
RestrictedEvaluationImplementation::RestrictedEvaluationImplementation()
  : NumericalMathEvaluationImplementation()
  , p_evaluation_(NoNumericalMathEvaluationImplementation().clone())
  , restrictionIndices_(0)
  , referencePoint_(0)
{
  // Nothing to do
} // RestrictedEvaluationImplementation

/* Default constructor */
RestrictedEvaluationImplementation::RestrictedEvaluationImplementation(const NumericalMathEvaluationImplementation & evaluation,
                                                                       const Indices & restrictionIndices,
                                                                       const NumericalPoint & referencePoint)
  : NumericalMathEvaluationImplementation()
  , p_evaluation_(evaluation.clone())
  , restrictionIndices_(restrictionIndices)
  , referencePoint_(referencePoint)
{
  if (referencePoint.getDimension() != evaluation.getInputDimension()) throw InvalidDimensionException(HERE) << "Error: the dimension of the reference point=" << referencePoint.getDimension() << " must match the input dimension of the evaluation=" << evaluation.getInputDimension();
  if (!restrictionIndices.check(referencePoint.getDimension() - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal function must be in the range [0, dim-1] and  must be different";
  const Description inputDescription(evaluation.getInputDescription());
  const Description outputDescription(evaluation.getOutputDescription());
  Description description;
  for (UnsignedLong i = 0; i < restrictionIndices.getSize(); ++i) description.add(inputDescription[restrictionIndices[i]]);
  for (UnsignedLong i = 0; i < outputDescription.getSize(); ++i) description.add(outputDescription[i]);
  setDescription(description);
} // RestrictedEvaluationImplementation

/* Default constructor */
RestrictedEvaluationImplementation::RestrictedEvaluationImplementation(const Implementation & p_evaluation,
                                                                       const Indices & restrictionIndices,
                                                                       const NumericalPoint & referencePoint)
  : NumericalMathEvaluationImplementation()
  , p_evaluation_(p_evaluation)
  , restrictionIndices_(restrictionIndices)
  , referencePoint_(referencePoint)
{
  if (referencePoint.getDimension() != p_evaluation->getInputDimension()) throw InvalidDimensionException(HERE) << "Error: the dimension of the reference point=" << referencePoint.getDimension() << " must match the input dimension of the evaluation=" << p_evaluation->getInputDimension();
  if (!restrictionIndices.check(referencePoint.getDimension() - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal function must be in the range [0, dim-1] and  must be different";
  const Description inputDescription(p_evaluation->getInputDescription());
  const Description outputDescription(p_evaluation->getOutputDescription());
  Description description;
  for (UnsignedLong i = 0; i < restrictionIndices.getSize(); ++i) description.add(inputDescription[restrictionIndices[i]]);
  for (UnsignedLong i = 0; i < outputDescription.getSize(); ++i) description.add(outputDescription[i]);
  setDescription(description);
} // RestrictedEvaluationImplementation


/* Virtual constructor */
RestrictedEvaluationImplementation * RestrictedEvaluationImplementation::clone() const
{
  return new RestrictedEvaluationImplementation(*this);
}


/* Comparison operator */
Bool RestrictedEvaluationImplementation::operator ==(const RestrictedEvaluationImplementation & other) const
{
  return true;
}

/* String converter */
String RestrictedEvaluationImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << RestrictedEvaluationImplementation::GetClassName()
      << " name=" << getName()
      << " evaluation=" << p_evaluation_->__repr__()
      << " restrictionIndices=" << restrictionIndices_
      << " referencePoint=" << referencePoint_;
  return oss;
}

/* String converter */
String RestrictedEvaluationImplementation::__str__(const String & offset) const
{
  OSS oss(false);
  oss << offset;
  if (hasVisibleName()) oss << getName() << " :";
  oss << __repr__();
  return oss;
}

/* Operator () */
NumericalPoint RestrictedEvaluationImplementation::operator() (const NumericalPoint & inP) const
{
  if (inP.getDimension() != getInputDimension()) throw InvalidArgumentException(HERE) << "Error: trying to evaluate a NumericalMathFunction with an argument of invalid dimension";
  NumericalPoint point(referencePoint_);
  for (UnsignedLong i = 0; i < restrictionIndices_.getSize(); ++i) point[restrictionIndices_[i]] = inP[i];
  return (*p_evaluation_)(point);
}

/* Accessor for input point dimension */
UnsignedLong RestrictedEvaluationImplementation::getInputDimension() const
{
  return restrictionIndices_.getSize();
}

/* Accessor for output point dimension */
UnsignedLong RestrictedEvaluationImplementation::getOutputDimension() const
{
  return p_evaluation_->getOutputDimension();
}

/* Accessors to the evaluation */
RestrictedEvaluationImplementation::Implementation RestrictedEvaluationImplementation::getEvaluation() const
{
  return p_evaluation_;
}

void RestrictedEvaluationImplementation::setEvaluation(const NumericalMathEvaluationImplementation & evaluation)
{
  if (referencePoint_.getDimension() != evaluation.getInputDimension()) throw InvalidDimensionException(HERE) << "Error: the input dimension of the evaluation=" << evaluation.getInputDimension() << " must match the dimension of the reference point=" << referencePoint_.getDimension();
  p_evaluation_ = evaluation.clone();
}

/* Accessors to the restriction indices */
Indices RestrictedEvaluationImplementation::getRestrictionIndices() const
{
  return restrictionIndices_;
}

void RestrictedEvaluationImplementation::setRestrictionIndices(const Indices & restrictionIndices)
{
  if (!restrictionIndices.check(referencePoint_.getDimension() - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal function must be in the range [0, dim-1] and  must be different";
  restrictionIndices_ = restrictionIndices;
}

/* Accessors to the reference point */
NumericalPoint RestrictedEvaluationImplementation::getReferencePoint() const
{
  return referencePoint_;
}

void RestrictedEvaluationImplementation::setReferencePoint(const NumericalPoint & referencePoint)
{
  if (referencePoint.getDimension() != p_evaluation_->getInputDimension()) throw InvalidDimensionException(HERE) << "Error: the dimension of the reference point=" << referencePoint.getDimension() << " must match the input dimension of the evaluation=" << p_evaluation_->getInputDimension();
  referencePoint_ = referencePoint;
}

/* Method save() stores the object through the StorageManager */
void RestrictedEvaluationImplementation::save(Advocate & adv) const
{
  NumericalMathEvaluationImplementation::save(adv);
  adv.saveAttribute( "evaluation_", *p_evaluation_ );
  adv.saveAttribute( "restrictionIndices_", restrictionIndices_ );
  adv.saveAttribute( "referencePoint_", referencePoint_ );
}

/* Method load() reloads the object from the StorageManager */
void RestrictedEvaluationImplementation::load(Advocate & adv)
{
  TypedInterfaceObject<NumericalMathEvaluationImplementation> evaluationValue;
  NumericalMathEvaluationImplementation::load(adv);
  adv.loadAttribute( "evaluation_", evaluationValue );
  adv.loadAttribute( "restrictionIndices_", restrictionIndices_ );
  adv.loadAttribute( "referencePoint_", referencePoint_ );
  p_evaluation_ = evaluationValue.getImplementation();
}

END_NAMESPACE_OPENTURNS
