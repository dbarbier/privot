//                                               -*- C++ -*-
/**
 * @file  RestrictedHessianImplementation.hxx
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
 * @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */

#ifndef OPENTURNS_RESTRICTEDHESSIANIMPLEMENTATION_HXX
#define OPENTURNS_RESTRICTEDHESSIANIMPLEMENTATION_HXX

#include "NumericalMathHessianImplementation.hxx"
#include "Pointer.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class RestrictedHessianImplementation
 *
 * The class that implement the marginal hessian of an hessian
 */
class RestrictedHessianImplementation
  : public NumericalMathHessianImplementation
{
  CLASSNAME;
public:

  /** Default constructor */
  RestrictedHessianImplementation();

  /** Default constructor */
  RestrictedHessianImplementation(const NumericalMathHessianImplementation & hessian,
                                  const Indices & restrictionIndices,
                                  const NumericalPoint & referencePoint);

  /** Virtual constructor */
  virtual RestrictedHessianImplementation * clone() const;

  /** Comparison operator */
  Bool operator ==(const RestrictedHessianImplementation & other) const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Hessian */
  using NumericalMathHessianImplementation::hessian;
  SymmetricTensor hessian(const NumericalPoint & inP) const;

  /** Accessor for input point dimension */
  UnsignedLong getInputDimension() const;

  /** Accessor for output point dimension */
  UnsignedLong getOutputDimension() const;

  /** Accessors to the hessian */
  Implementation getHessian() const;
  void setHessian(const NumericalMathHessianImplementation & hessian);

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

  /** The underlying hessian */
  Implementation p_hessian_;

  /** The indices of the free input variables */
  Indices restrictionIndices_;

  /** Reference point from which the values of the fixed components are taken */
  NumericalPoint referencePoint_;

}; /* class RestrictedHessianImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_RESTRICTEDHESSIANIMPLEMENTATION_HXX */
