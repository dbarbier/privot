//                                               -*- C++ -*-
/**
 *  @file  ProductPolynomialGradientImplementation.hxx
 *  @brief This is a nD polynomial build as a product of n 1D polynomial
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
#ifndef OPENTURNS_PRODUCTPOLYNOMIALGRADIENTIMPLEMENTATION_HXX
#define OPENTURNS_PRODUCTPOLYNOMIALGRADIENTIMPLEMENTATION_HXX

#include "NumericalMathGradientImplementation.hxx"
#include "UniVariatePolynomial.hxx"
#include "PersistentCollection.hxx"
#include "NumericalPoint.hxx"


BEGIN_NAMESPACE_OPENTURNS



/**
 * @class ProductPolynomialGradientImplementation
 *
 * This is a nD polynomial build as a product of n 1D polynomial
 */

class ProductPolynomialGradientImplementation
  : public NumericalMathGradientImplementation
{
  CLASSNAME;
public:

  typedef Collection<UniVariatePolynomial>           PolynomialCollection;
  typedef PersistentCollection<UniVariatePolynomial> PolynomialPersistentCollection;

  /** Constructor */
  ProductPolynomialGradientImplementation(const PolynomialCollection & coll);

  /** Virtual constructor */
  ProductPolynomialGradientImplementation * clone() const;

  /** String converter */
  virtual String __repr__() const;

  /** Compute the gradient of a product of univariate polynomials */
  virtual Matrix gradient(const NumericalPoint & inP) const;

  /** Accessor for input point dimension */
  virtual UnsignedLong getInputDimension() const;

  /** Accessor for output point dimension */
  virtual UnsignedLong getOutputDimension() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);



protected:

  friend class Factory<ProductPolynomialGradientImplementation>;

  /* Default constructor */
  ProductPolynomialGradientImplementation();

private:

  /** The set of 1d polynomials to build the nD polynomial */
  PolynomialPersistentCollection polynomials_;

} ; /* class ProductPolynomialGradientImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_PRODUCTPOLYNOMIALGRADIENTIMPLEMENTATION_HXX */
