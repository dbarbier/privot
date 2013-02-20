//                                               -*- C++ -*-
/**
 * @file  RestrictedGradientImplementation.hxx
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
 * @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */

#ifndef OPENTURNS_RESTRICTEDGRADIENTIMPLEMENTATION_HXX
#define OPENTURNS_RESTRICTEDGRADIENTIMPLEMENTATION_HXX

#include "NumericalMathGradientImplementation.hxx"
#include "Pointer.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class RestrictedGradientImplementation
 *
 * The class that implement the marginal gradient of an gradient
 */
class RestrictedGradientImplementation
  : public NumericalMathGradientImplementation
{
  CLASSNAME;
public:

  /** Default constructor */
  RestrictedGradientImplementation();

  /** Default constructor */
  RestrictedGradientImplementation(const NumericalMathGradientImplementation & gradient,
                                   const Indices & restrictionIndices,
                                   const NumericalPoint & referencePoint);

  /** Virtual constructor */
  virtual RestrictedGradientImplementation * clone() const;

  /** Comparison operator */
  Bool operator ==(const RestrictedGradientImplementation & other) const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Gradient */
  using NumericalMathGradientImplementation::gradient;
  Matrix gradient (const NumericalPoint & inP) const;

  /** Accessor for input point dimension */
  UnsignedLong getInputDimension() const;

  /** Accessor for output point dimension */
  UnsignedLong getOutputDimension() const;

  /** Accessors to the gradient */
  Implementation getGradient() const;
  void setGradient(const NumericalMathGradientImplementation & gradient);

  /** Accessors to the restriction indices */
  Indices getRestrictionIndices() const;
  void setRestrictionIndices(const Indices & restrictionIndices);

  /** Accessors to the reference point */
  NumericalPoint getReferencePoint() const;
  void setReferencePoint(const NumericalPoint & referencePoint);

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

private:

  /** The underlying gradient */
  Implementation p_gradient_;

  /** The indices of the free input variables */
  Indices restrictionIndices_;

  /** Reference point from which the values of the fixed components are taken */
  NumericalPoint referencePoint_;

}; /* class RestrictedGradientImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_RESTRICTEDGRADIENTIMPLEMENTATION_HXX */
