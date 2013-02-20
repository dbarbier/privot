//                                               -*- C++ -*-
/**
 *  @file  ARMACoefficients.hxx
 *  @brief ARMACoefficients class enables to stock coefficients of an ARMA process
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
 *  @author schueller
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 *  Id      ARMACoefficients.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_ARMACOEFFICIENTS_HXX
#define OPENTURNS_ARMACOEFFICIENTS_HXX

#include "PersistentCollection.hxx"
#include "SquareMatrix.hxx"
#include "NumericalPoint.hxx"
#include "Collection.hxx"
#include "UniVariatePolynomial.hxx"



BEGIN_NAMESPACE_OPENTURNS




/**
 * @class ARMACoefficients
 *
 * Coefficients of ARMA process
 *
 */
class ARMACoefficients
  : public PersistentCollection<SquareMatrix>
{
  CLASSNAME;

public:

  /** Some typedefs to ease reading */
  typedef Collection<SquareMatrix>             SquareMatrixCollection;
  typedef PersistentCollection<SquareMatrix>   SquareMatrixPersistentCollection;

  /** Default and standard constructors */
  ARMACoefficients(const UnsignedLong & size = 0,
                   const UnsignedLong & dimension = 1);
  ARMACoefficients(const SquareMatrixCollection & collection);
  ARMACoefficients(const NumericalPoint & ScalarCoefficients);
  ARMACoefficients(const UniVariatePolynomial & polynomial);

  /** Virtual constructor */
  virtual ARMACoefficients * clone() const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** Dimension accessor */
  UnsignedLong getDimension() const ;

  /** Definition of add method : control of SquareMatrix sizes */
  void add(const SquareMatrix & matrix);

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:

  /** Dimension of the underlying ARMA process */
  UnsignedLong dimension_;

}; /* class ARMACoefficients */
END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ARMACOEFFICIENTS_HXX */
