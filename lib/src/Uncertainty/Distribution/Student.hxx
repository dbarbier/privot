//                                               -*- C++ -*-
/**
 *  @file  Student.hxx
 *  @brief The Student distribution
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
 *  Id      Student.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_STUDENT_HXX
#define OPENTURNS_STUDENT_HXX

#include "OTprivate.hxx"
#include "EllipticalDistribution.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class Student
 *
 * The Student distribution.
 */
class Student
  : public EllipticalDistribution
{

  CLASSNAME;
public:

  typedef Pointer<DistributionImplementation> Implementation;

  /** Default constructor */
  Student(const NumericalScalar nu = 3.0,
          const UnsignedLong dimension = 1);

  /** Parameters constructor */
  Student(const NumericalScalar nu,
          const NumericalScalar mu,
          const NumericalScalar sigma = 1.0);

  /** Parameters constructor */
  Student(const NumericalScalar nu,
          const NumericalPoint & mu,
          const NumericalPoint & sigma,
          const CorrelationMatrix & R);


  /** Comparison operator */
  Bool operator ==(const Student & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;



  /* Interface inherited from Distribution */

  /** Virtual constructor */
  virtual Student * clone() const;

  /** Get one realization of the distribution */
  NumericalPoint getRealization() const;

  /** Get the CDF of the distribution */
  using EllipticalDistribution::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;
  using EllipticalDistribution::computeComplementaryCDF;
  NumericalScalar computeComplementaryCDF(const NumericalPoint & point) const;

  /** Get the probability content of an interval */
  NumericalScalar computeProbability(const Interval & interval) const;

  /** Get the PDFGradient of the distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDFGradient of the distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Compute the radial distribution CDF */
  NumericalScalar computeRadialDistributionCDF(const NumericalScalar radius,
                                               const Bool tail = false) const;

  /** Compute the PDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
  NumericalScalar computeConditionalPDF(const NumericalScalar x, const NumericalPoint & y) const;

  /** Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
  NumericalScalar computeConditionalCDF(const NumericalScalar x, const NumericalPoint & y) const;

  /** Compute the quantile of Xi | X1, ..., Xi-1, i.e. x such that CDF(x|y) = q with x = Xi, y = (X1,...,Xi-1) */
  NumericalScalar computeConditionalQuantile(const NumericalScalar q, const NumericalPoint & y) const;

  /** Get the i-th marginal distribution */
  Implementation getMarginal(const UnsignedLong i) const;

  /** Get the distribution of the marginal distribution corresponding to indices dimensions */
  Implementation getMarginal(const Indices & indices) const;

  /** Get the mean of the distribution */
  NumericalPoint getMean() const;

  /** Get the standard deviation of the distribution */
  NumericalPoint getStandardDeviation() const;

  /** Get the skewness of the distribution */
  NumericalPoint getSkewness() const;

  /** Get the kurtosis of the distribution */
  NumericalPoint getKurtosis() const;

  /** Get the covariance of the distribution */
  CovarianceMatrix getCovariance() const;

  /** Get the raw moments of the standardized distribution */
  NumericalPoint getStandardMoment(const UnsignedLong n) const;

  /** Get the standard representative in the parametric family, associated with the standard moments */
  Implementation getStandardRepresentative() const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;
  using EllipticalDistribution::setParametersCollection;
  void setParametersCollection(const NumericalPointCollection & parametersCollection);

  /** Tell if the distribution has independent copula */
  Bool hasIndependentCopula() const;

  /* Interface specific to Student */

  /** Nu accessor */
  void setNu(const NumericalScalar nu);
  NumericalScalar getNu() const;

  /** Mu accessor */
  void setMu(const NumericalScalar mu);
  NumericalScalar getMu() const;

  /** Compute the density generator of the ellipticalal generator, i.e.
   *  the function phi such that the density of the distribution can
   *  be written as p(x) = phi(t(x-mu)R^(-1)(x-mu))                      */
  NumericalScalar computeDensityGenerator(const NumericalScalar betaSquare) const;

  /** Compute the derivative of the density generator */
  NumericalScalar computeDensityGeneratorDerivative(const NumericalScalar betaSquare) const;

  /** Compute the seconde derivative of the density generator */
  NumericalScalar computeDensityGeneratorSecondDerivative(const NumericalScalar betaSquare) const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

private:

  /** Quantile computation for dimension=1 */
  NumericalScalar computeScalarQuantile(const NumericalScalar prob,
                                        const Bool tail = false,
                                        const NumericalScalar precision = ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon")) const;

  /** The nu of the Student distribution */
  NumericalScalar nu_;

  /** Specific normalization factor for the Student distribution */
  NumericalScalar studentNormalizationFactor_;
}; /* class Student */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_STUDENT_HXX */
