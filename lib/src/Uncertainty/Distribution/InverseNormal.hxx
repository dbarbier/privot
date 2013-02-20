//                                               -*- C++ -*-
/**
 *  @file  InverseNormal.hxx
 *  @brief The InverseNormal distribution
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
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 *  Id      InverseNormal.hxx
 */
#ifndef OPENTURNS_INVERSENORMAL_HXX
#define OPENTURNS_INVERSENORMAL_HXX

#include "OTprivate.hxx"
#include "NonEllipticalDistribution.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class InverseNormal
 *
 * The InverseNormal distribution.
 */
class InverseNormal
  : public NonEllipticalDistribution
{
  CLASSNAME;
public:

  /** Default constructor */
  InverseNormal();

  /** Parameters constructor */
  InverseNormal(const NumericalScalar lambda,
                const NumericalScalar mu);


  /** Comparison operator */
  Bool operator ==(const InverseNormal & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;


  /* Interface inherited from Distribution */

  /** Virtual constructor */
  InverseNormal * clone() const;

  /** Get one realization of the InverseNormal distribution */
  NumericalPoint getRealization() const;

  /** Get the PDF of the InverseNormal distribution */
  using NonEllipticalDistribution::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;
  using NonEllipticalDistribution::computeLogPDF;
  NumericalScalar computeLogPDF(const NumericalPoint & point) const;

  /** Get the CDF of the InverseNormal distribution */
  using NonEllipticalDistribution::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;

  /** Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
  NumericalComplex computeCharacteristicFunction(const NumericalScalar x) const;
  NumericalComplex computeLogCharacteristicFunction(const NumericalScalar x) const;

  /** Get the standard deviation of the distribution */
  NumericalPoint getStandardDeviation() const;

  /** Get the skewness of the distribution */
  NumericalPoint getSkewness() const;

  /** Get the kurtosis of the distribution */
  NumericalPoint getKurtosis() const;

  /** Get the raw moments of the standardized distribution */
  NumericalPoint getStandardMoment(const UnsignedLong n) const;

  /* Interface specific to InverseNormal */
  void setLambdaMu(const NumericalScalar lambda,
                   const NumericalScalar mu);
  /** lambda accessor */
  NumericalScalar getLambda() const;

  /** mu accessor */
  NumericalScalar getMu() const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;
  using NonEllipticalDistribution::setParametersCollection;
  void setParametersCollection(const NumericalPointCollection & parametersCollection);

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

private:

  /** Compute the numerical range of the distribution given the parameters values */
  void computeRange();

  /** Compute the mean of the distribution */
  void computeMean() const;

  /** Compute the covariance of the distribution */
  void computeCovariance() const;

  /** The main parameter set of the distribution */
  NumericalScalar lambda_;
  NumericalScalar mu_;

}; /* class InverseNormal */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_INVERSENORMAL_HXX */
