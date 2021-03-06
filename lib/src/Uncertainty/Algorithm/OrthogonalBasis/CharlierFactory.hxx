//                                               -*- C++ -*-
/**
 *  @file  CharlierFactory.hxx
 *  @brief Charlier polynomial factory
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
#ifndef OPENTURNS_CHARLIERFACTORY_HXX
#define OPENTURNS_CHARLIERFACTORY_HXX

#include "OrthogonalUniVariatePolynomialFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class CharlierFactory
 *
 * Charlier polynomial factory
 */

class CharlierFactory
  : public OrthogonalUniVariatePolynomialFactory
{
  CLASSNAME;
public:


  /** Default constructor, associated with the default Poisson distribution of parameter 1. */
  CharlierFactory();

  /** Parameter constructor: lambda is the order of the generalized Charlier polynomial, associated with the Poisson(lambda) distribution */
  CharlierFactory(const NumericalScalar lambda);

  /** Virtual constructor */
  CharlierFactory * clone() const;

  /** Calculate the coefficients of recurrence a0n, a1n, a2n such that
      Pn+1(x) = (a0n * x + a1n) * Pn(x) + a2n * Pn-1(x) */
  Coefficients getRecurrenceCoefficients(const UnsignedLong n) const;

  /** K accessor */
  NumericalScalar getLambda() const;

  /** String converter */
  String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

private:
  /** Parameter of the Charlier polynomial */
  NumericalScalar lambda_;

} ; /* class CharlierFactory */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_CHARLIERFACTORY_HXX */
