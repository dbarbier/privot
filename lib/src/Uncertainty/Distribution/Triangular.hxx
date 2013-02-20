//                                               -*- C++ -*-
/**
 *  @file  Triangular.hxx
 *  @brief The Triangular distribution
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 *  Id      Triangular.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_TRIANGULAR_HXX
#define OPENTURNS_TRIANGULAR_HXX

#include "OTprivate.hxx"
#include "NonEllipticalDistribution.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class Triangular
 *
 * The Triangular distribution.
 */
class Triangular
  : public NonEllipticalDistribution
{
  CLASSNAME;
public:

  typedef Pointer<DistributionImplementation> Implementation;

  /** Default constructor */
  Triangular();

  /** Parameters constructor */
  Triangular(const NumericalScalar a,
             const NumericalScalar m,
             const NumericalScalar b);


  /** Comparison operator */
  Bool operator ==(const Triangular & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;



  /* Interface inherited from Distribution */

  /** Virtual constructor */
  Triangular * clone() const;

  /** Get one realization of the Triangular distribution */
  NumericalPoint getRealization() const;

  /** Get the DDF of the Triangular distribution */
  using NonEllipticalDistribution::computeDDF;
  NumericalPoint computeDDF(const NumericalPoint & point) const;

  /** Get the PDF of the Triangular distribution */
  using NonEllipticalDistribution::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;

  /** Get the CDF of the Triangular distribution */
  using NonEllipticalDistribution::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;

  /** Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
  NumericalComplex computeCharacteristicFunction(const NumericalScalar x) const;
  NumericalComplex computeLogCharacteristicFunction(const NumericalScalar x) const;

  /** Get the PDFGradient of the Triangular distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDFGradient of the Triangular distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Get the roughness, i.e. the L2-norm of the PDF */
  NumericalScalar getRoughness() const;

  /** Get the standard deviation of the distribution */
  NumericalPoint getStandardDeviation() const;

  /** Get the skewness of the distribution */
  NumericalPoint getSkewness() const;

  /** Get the kurtosis of the distribution */
  NumericalPoint getKurtosis() const;

  /** Get the raw moments of the standardized distribution */
  NumericalPoint getStandardMoment(const UnsignedLong n) const;

  /** Get the standard representative in the parametric family, associated with the standard moments */
  Implementation getStandardRepresentative() const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;
  using NonEllipticalDistribution::setParametersCollection;
  void setParametersCollection(const NumericalPointCollection & parametersCollection);


  /* Interface specific to Triangular */

  /** A accessor */
  void setAMB(const NumericalScalar a,
              const NumericalScalar m,
              const NumericalScalar b);
  NumericalScalar getA() const;

  /** M accessor */
  NumericalScalar getM() const;

  /** B accessor */
  NumericalScalar getB() const;

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

  /** Get the quantile of the Triangular distribution */
  NumericalScalar computeScalarQuantile(const NumericalScalar prob,
                                        const Bool tail = false,
                                        const NumericalScalar precision = ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon")) const;

  /** Compute the numerical range of the distribution given the parameters values */
  void computeRange();

  /** The main parameter set of the distribution */
  NumericalScalar a_;
  NumericalScalar m_;
  NumericalScalar b_;

}; /* class Triangular */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_TRIANGULAR_HXX */
