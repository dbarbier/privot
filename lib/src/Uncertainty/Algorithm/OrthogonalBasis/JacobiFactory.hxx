//                                               -*- C++ -*-
/**
 *  @file  JacobiFactory.hxx
 *  @brief Jacobi polynomial factory
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
#ifndef OPENTURNS_JACOBIFACTORY_HXX
#define OPENTURNS_JACOBIFACTORY_HXX

#include "OrthogonalUniVariatePolynomialFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class JacobiFactory
 *
 * Jacobi polynomial factory
 */

class JacobiFactory
  : public OrthogonalUniVariatePolynomialFactory
{
  CLASSNAME;
public:


  /** Default constructor: (1, 1) order Jacobi polynomial associated with the default Beta() = Beta(2, 4, -1, 1) distribution which is equal to the Epanechnikov distribution */
  JacobiFactory();

  /** Parameter constructor: (alpha, beta) is the order of the Jacobi polynomial, associated with the Beta(beta + 1, alpha + beta + 2, -1, 1) distribution in the ANALYSIS parameter set or to the Beta(alpha, beta, -1, 1) distribution in the PROBABILITY parameter set */
  JacobiFactory(const NumericalScalar alpha,
                const NumericalScalar beta,
                const ParameterSet parameterization = ANALYSIS);

  /** Virtual constructor */
  JacobiFactory * clone() const;

  /** Calculate the coefficients of recurrence a0n, a1n, a2n such that
      Pn+1(x) = (a0n * x + a1n) * Pn(x) + a2n * Pn-1(x) */
  Coefficients getRecurrenceCoefficients(const UnsignedLong n) const;

  /** Alpha accessor */
  NumericalScalar getAlpha() const;

  /** Beta accessor */
  NumericalScalar getBeta() const;

  /** String converter */
  String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

private:
  /* First parameter of the Jacobi polynomial */
  NumericalScalar alpha_;
  /* Second parameter of the Jacobi polynomial */
  NumericalScalar beta_;

} ; /* class JacobiFactory */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_JACOBIFACTORY_HXX */
