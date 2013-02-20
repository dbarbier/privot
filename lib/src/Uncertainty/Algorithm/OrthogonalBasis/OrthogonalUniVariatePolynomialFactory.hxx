//                                               -*- C++ -*-
/**
 *  @file  OrthogonalUniVariatePolynomialFactory.hxx
 *  @brief This an abstract class for 1D polynomial factories
 *
 *  Copyright (C) 2005-2013 EDF-EADS-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @author dutka
 *  @date   2008-05-21 17:44:02 +0200 (Wed, 21 May 2008)
 *  Id      Object.hxx 818 2008-05-21 15:44:02Z dutka
 */
#ifndef OPENTURNS_ORTHOGONALUNIVARIATEPOLYNOMIALFACTORY_HXX
#define OPENTURNS_ORTHOGONALUNIVARIATEPOLYNOMIALFACTORY_HXX

#include "PersistentObject.hxx"
#include "NumericalPoint.hxx"
#include "PersistentCollection.hxx"
#include "OrthogonalUniVariatePolynomial.hxx"
#include "Distribution.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class OrthogonalUniVariatePolynomialFactory
 *
 * This an abstract class for 1D polynomial factories
 */

class OrthogonalUniVariatePolynomialFactory
  : public PersistentObject
{
  CLASSNAME;

public:

  typedef OrthogonalUniVariatePolynomial::Coefficients Coefficients;
  typedef Collection<Coefficients>                     CoefficientsCollection;
  typedef PersistentCollection<Coefficients>           CoefficientsPersistentCollection;

  enum ParameterSet { ANALYSIS, PROBABILITY };

  /** Constructor */
  OrthogonalUniVariatePolynomialFactory(const Distribution & measure);

  /** Virtual constructor */
  virtual OrthogonalUniVariatePolynomialFactory * clone() const;

  /** String converter */
  virtual String __repr__() const;

  /** The method to get the polynomial of any degree. */
  OrthogonalUniVariatePolynomial build(const UnsignedLong degree) const;

  /** Build the 3 terms recurrence coefficients up to the needed degree */
  CoefficientsCollection buildRecurrenceCoefficientsCollection(const UnsignedLong degree) const;

  /** Build the coefficients of the polynomial of the needed degree */
  Coefficients buildCoefficients(const UnsignedLong degree) const;

  /** Roots of the polynomial of degree n as the eigenvalues of the associated matrix */
  NumericalPoint getRoots(const UnsignedLong n) const;

  /** Nodes and weights of the polynomial of degree n as the eigenvalues of the associated matrix, to build quadrature rules */
  NumericalPoint getNodesAndWeights(const UnsignedLong n,
                                    NumericalPoint & weights) const;

  /** Measure accessor */
  Distribution getMeasure() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);


  /** Calculate the coefficients of recurrence a0, a1, a2 such that
      Pn+1(x) = (a0 * x + a1) * Pn(x) + a2 * Pn-1(x) */
  virtual Coefficients getRecurrenceCoefficients(const UnsignedLong n) const;

protected:

  friend class Factory<OrthogonalUniVariatePolynomialFactory>;

  /** Default constructor */
  OrthogonalUniVariatePolynomialFactory();

  /** Cache initialization */
  virtual void initializeCache();

  /** The distribution of the particular Orthonormal polynomial */
  Distribution measure_;

  /** A cache to save already computed coefficients */
  mutable CoefficientsPersistentCollection coefficientsCache_;

  /** A cache to save already computed recurrence coefficients */
  mutable CoefficientsPersistentCollection recurrenceCoefficientsCache_;

} ; /* class OrthogonalUniVariatePolynomialFactory */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ORTHOGONALUNIVARIATEPOLYNOMIALFACTORY_HXX */
