//                                               -*- C++ -*-
/**
 *  @file  MeixnerFactory.hxx
 *  @brief Meixner polynomial factory
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
 *  @author schueller
 *  @date   2008-05-21 17:44:02 +0200 (Wed, 21 May 2008)
 *  Id      Object.hxx 818 2008-05-21 15:44:02Z dutka
 */
#ifndef OPENTURNS_MEIXNERFACTORY_HXX
#define OPENTURNS_MEIXNERFACTORY_HXX

#include "OrthogonalUniVariatePolynomialFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class MeixnerFactory
 *
 * Meixner polynomial factory
 */

class MeixnerFactory
  : public OrthogonalUniVariatePolynomialFactory
{
  CLASSNAME;
public:


  /** Default constructor, associated with the default Negative Binomial distribution of parameter 1, 1/2. */
  MeixnerFactory();

  /** Parameter constructor: lambda is the order of the generalized Meixner polynomial, associated with the NegativeBinomial(r, p) distribution */
  MeixnerFactory(const NumericalScalar r,
                 const NumericalScalar p);

  /** Virtual constructor */
  MeixnerFactory * clone() const;

  /** Calculate the coefficients of recurrence a0n, a1n, a2n such that
      Pn+1(x) = (a0n * x + a1n) * Pn(x) + a2n * Pn-1(x) */
  Coefficients getRecurrenceCoefficients(const UnsignedLong n) const;

  /** R accessor */
  NumericalScalar getR() const;

  /** P accessor */
  NumericalScalar getP() const;

  /** String converter */
  String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

private:
  /** Parameters of the Meixner polynomial */
  NumericalScalar r_;
  NumericalScalar p_;

} ; /* class MeixnerFactory */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_MEIXNERFACTORY_HXX */
