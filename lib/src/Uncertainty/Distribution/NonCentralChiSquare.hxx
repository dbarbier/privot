//                                               -*- C++ -*-
/**
 *  @file  NonCentralChiSquare.hxx
 *  @brief The NonCentralChiSquare distribution
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
 *  @author lebrun
 *  @date   2011-02-26 21:12:17 +0100 (sam. 26 févr. 2011)
 *  Id      NonCentralChiSquare.hxx 1813 2011-02-26 20:12:17Z lebrun
 */
#ifndef OPENTURNS_NONCENTRALCHISQUARE_HXX
#define OPENTURNS_NONCENTRALCHISQUARE_HXX

#include "NonEllipticalDistribution.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class NonCentralChiSquare
 *
 * The NonCentralChiSquare distribution.
 */
class NonCentralChiSquare
  : public NonEllipticalDistribution
{
  CLASSNAME;
public:

  /** Default constructor */
  /* The default values correspond to a classical ChiSquare distribution with 5 degrees of freedom */
  NonCentralChiSquare(const NumericalScalar nu = 5.0,
                      const NumericalScalar lambda = 0.0);

  /** Comparison operator */
  Bool operator ==(const NonCentralChiSquare & other) const;

  /** String converter */
  String __repr__() const;

  String __str__(const String & offset = "") const;


  /* Interface inherited from Distribution */

  /** Virtual constructor */
  NonCentralChiSquare * clone() const;

  /** Get one realization of the distribution */
  NumericalPoint getRealization() const;

  /** Get the PDF of the distribution */
  using NonEllipticalDistribution::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;

  /** Get the CDF of the distribution */
  using NonEllipticalDistribution::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;

  /** Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
  NumericalComplex computeCharacteristicFunction(const NumericalScalar x) const;
  NumericalComplex computeLogCharacteristicFunction(const NumericalScalar x) const;

  /** Get the PDFGradient of the distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDFGradient of the distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Get the standard deviation of the distribution */
  NumericalPoint getStandardDeviation() const;

  /** Get the skewness of the distribution */
  NumericalPoint getSkewness() const;

  /** Get the kurtosis of the distribution */
  NumericalPoint getKurtosis() const;

  /** Get the raw moments of the standardized distribution */
  NumericalPoint getStandardMoment(const UnsignedLong n) const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;
  using NonEllipticalDistribution::setParametersCollection;
  void setParametersCollection(const NumericalPointCollection & parametersCollection);

  /** Nu accessor */
  void setNu(const NumericalScalar nu);
  void setNuLambda(const NumericalScalar nu,
                   const NumericalScalar lambda);
  NumericalScalar getNu() const;

  /** Lambda accessor */
  void setLambda(const NumericalScalar lambda);
  NumericalScalar getLambda() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);


protected:


private:

  /** Compute the mean of the distribution */
  void computeMean() const;

  /** Compute the covariance of the distribution */
  void computeCovariance() const;

  /** Number of degrees of freedom */
  NumericalScalar nu_;

  /** Non-centrality parameter */
  NumericalScalar lambda_;

}; /* class NonCentralChiSquare */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_NONCENTRALCHISQUARE_HXX */
