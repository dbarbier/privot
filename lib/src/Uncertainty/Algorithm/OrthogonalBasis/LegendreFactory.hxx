//                                               -*- C++ -*-
/**
 *  @file  LegendreFactory.hxx
 *  @brief Legendre polynomial factory
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
#ifndef OPENTURNS_LEGENDREFACTORY_HXX
#define OPENTURNS_LEGENDREFACTORY_HXX

#include "OrthogonalUniVariatePolynomialFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class LegendreFactory
 *
 * Legendre polynomial factory
 */

class LegendreFactory
  : public OrthogonalUniVariatePolynomialFactory
{
  CLASSNAME;
public:


  /** Default constructor */
  LegendreFactory();

  /** Virtual constructor */
  LegendreFactory * clone() const;

  /** Calculate the coefficients of recurrence a0n, a1n, a2n such that
      Pn+1(x) = (a0n * x + a1n) * Pn(x) + a2n * Pn-1(x) */
  Coefficients getRecurrenceCoefficients(const UnsignedLong n) const;

  /** String converter */
  String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

private:

} ; /* class LegendreFactory */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LEGENDREFACTORY_HXX */
