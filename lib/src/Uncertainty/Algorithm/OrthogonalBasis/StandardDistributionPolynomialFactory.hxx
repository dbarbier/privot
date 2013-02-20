//                                               -*- C++ -*-
/**
 *  @file  StandardDistributionPolynomialFactory.hxx
 *  @brief OrthogonalUniVariatePolynomialStandardDistribution polynomial factory
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
#ifndef OPENTURNS_STANDARDDISTRIBUTIONPOLYNOMIALFACTORY_HXX
#define OPENTURNS_STANDARDDISTRIBUTIONPOLYNOMIALFACTORY_HXX

#include <map>

#include "OrthogonalUniVariatePolynomialFactory.hxx"
#include "OrthonormalizationAlgorithm.hxx"
#include "Collection.hxx"
#include "OrthogonalUniVariatePolynomialFamily.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class StandardDistributionPolynomialFactory
 *
 * OrthogonalUniVariatePolynomialStandardDistribution polynomial factory
 */

class StandardDistributionPolynomialFactory
  : public OrthogonalUniVariatePolynomialFactory
{
  CLASSNAME;
public:


  /** Default constructor */
  StandardDistributionPolynomialFactory();

  /** Parameter constructor */
  StandardDistributionPolynomialFactory(const Distribution & distribution);

  /** Parameter constructor with specific orthonormalization algorithm */
  StandardDistributionPolynomialFactory(const OrthonormalizationAlgorithm & orthonormalizationAlgorithm);

  /** Virtual constructor */
  StandardDistributionPolynomialFactory * clone() const;

  /** Calculate the coefficients of recurrence a0, a1, a2 such that
      Pn+1(x) = (a0 * x + a1) * Pn(x) + a2 * Pn-1(x) */
  Coefficients getRecurrenceCoefficients(const UnsignedLong n) const;

  /** String converter */
  String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

private:

  /** Check the existence of a specific family more efficient for the given measure */
  void checkSpecificFamily();

  /** Algorithm used to perform the orthonormalization */
  OrthonormalizationAlgorithm orthonormalizationAlgorithm_;

  /** Specific factory if a special case is recognized */
  OrthogonalUniVariatePolynomialFamily specificFamily_;

  /** Flag to tell if a specific family is available */
  Bool hasSpecificFamily_;

} ; /* class StandardDistributionPolynomialFactory */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_STANDARDDISTRIBUTIONPOLYNOMIALFACTORY_HXX */
