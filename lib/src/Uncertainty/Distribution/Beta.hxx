//                                               -*- C++ -*-
/**
 *  @file  Beta.hxx
 *  @brief The Beta distribution
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
 *  Id      Beta.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_BETA_HXX
#define OPENTURNS_BETA_HXX

#include "NonEllipticalDistribution.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class Beta
 *
 * The Beta distribution.
 */
class Beta
  : public NonEllipticalDistribution
{
  CLASSNAME;
public:

  enum ParameterSet { RT, MUSIGMA };

  typedef Pointer<DistributionImplementation> Implementation;

  /** Default constructor */
  Beta();

  /** Parameters constructor */
  Beta(const NumericalScalar arg1,
       const NumericalScalar arg2,
       const NumericalScalar a,
       const NumericalScalar b,
       const ParameterSet set = RT);

  /** Comparison operator */
  Bool operator ==(const Beta & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;


  /* Interface inherited from Distribution */

  /** Virtual constructor */
  Beta * clone() const;

  /** Get one realization of the distribution */
  NumericalPoint getRealization() const;

  /** Get the DDF of the distribution */
  using NonEllipticalDistribution::computeDDF;
  NumericalPoint computeDDF(const NumericalPoint & point) const;

  /** Get the PDF of the distribution */
  using NonEllipticalDistribution::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;
  using NonEllipticalDistribution::computeLogPDF;
  NumericalScalar computeLogPDF(const NumericalPoint & point) const;

  /** Get the CDF of the distribution */
  using NonEllipticalDistribution::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;

  /** Get the PDFGradient of the distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDFGradient of the distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
  NumericalComplex computeCharacteristicFunction(const NumericalScalar x) const;

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

  /** Check if the distribution is elliptical */
  Bool isElliptical() const;


  /* Interface specific to Beta */

  /** R accessor */
  void setR(const NumericalScalar r);
  NumericalScalar getR() const;

  /** T accessor */
  void setT(const NumericalScalar t);
  NumericalScalar getT() const;

  /** Mu accessor */
  void setMuSigma(const NumericalScalar mu,
                  const NumericalScalar sigma);
  NumericalScalar getMu() const;

  /** Sigma accessor */
  NumericalScalar getSigma() const;

  /** A accessor */
  void setA(const NumericalScalar a);
  NumericalScalar getA() const;

  /** B accessor */
  void setB(const NumericalScalar b);
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

  /** Get the quantile of the distribution */
  NumericalScalar computeScalarQuantile(const NumericalScalar prob,
                                        const Bool tail = false,
                                        const NumericalScalar precision = ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon")) const;

  /** RT accessor that avoid a check between the setting of r and the setting of t */
  void setRT(const NumericalScalar r,
             const NumericalScalar t);

  /** Compute the numerical range of the distribution given the parameters values */
  void computeRange();

  /** Update the derivative attributes */
  void update();

  /** The main parameter set of the distribution */
  NumericalScalar r_;
  NumericalScalar t_;
  NumericalScalar a_;
  NumericalScalar b_;
  NumericalScalar normalizationFactor_;
}; /* class Beta */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_BETA_HXX */
