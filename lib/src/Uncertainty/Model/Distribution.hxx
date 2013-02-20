//                                               -*- C++ -*-
/**
 *  @file  Distribution.hxx
 *  @brief Abstract top-level class for all distributions
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
 *  Id      Distribution.hxx 2608 2012-07-16 13:59:45Z schueller
 */
#ifndef OPENTURNS_DISTRIBUTION_HXX
#define OPENTURNS_DISTRIBUTION_HXX

#include "TypedInterfaceObject.hxx"
#include "DistributionImplementation.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class Distribution
 *
 * The class describes the probabilistic concept of distribution.
 * Instances of this class can produce samples following the
 * distribution, can compute PDF or CDF, etc.
 * They are the actual key component of RandomVectors.
 */
class Distribution
  : public TypedInterfaceObject<DistributionImplementation>
{
  CLASSNAME;
public:

  typedef NumericalMathFunction                     IsoProbabilisticTransformation;
  typedef IsoProbabilisticTransformation            InverseIsoProbabilisticTransformation;
  typedef Collection<NumericalPoint>                NumericalPointCollection;
  typedef Collection<NumericalPointWithDescription> NumericalPointWithDescriptionCollection;

  /** Default constructor */
  Distribution();

  /** Default constructor */
  Distribution(const DistributionImplementation & implementation,
               const String & name = DefaultName);


  /** Constructor from implementation */
  Distribution(const Implementation & p_implementation,
               const String & name = DefaultName);

#ifndef SWIG
  /** Constructor from implementation pointer */
  Distribution(DistributionImplementation * p_implementation,
               const String & name = DefaultName);
#endif

  /** Comparison operator */
  Bool operator ==(const Distribution & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** Weight accessor */
  void setWeight(const NumericalScalar w);
  NumericalScalar getWeight() const;

  /** Dimension accessor */
  UnsignedLong getDimension() const;

  /* Here is the interface that all derived class must implement */

  /** Get one realization of the distribution */
  NumericalPoint getRealization() const;

  /** Get a numerical sample whose elements follow the distribution */
  NumericalSample getSample(const UnsignedLong size) const;

  /** Get a numerical sample whose elements follow the distribution
   * @deprecated
   */
  NumericalSample getNumericalSample(const UnsignedLong size) const;

  /** Get the DDF of the distribution */
  NumericalScalar computeDDF(const NumericalScalar scalar) const;
  NumericalPoint  computeDDF(const NumericalPoint & point) const;
  NumericalSample computeDDF(const NumericalSample & sample) const;

  /** Get the PDF of the distribution */
  NumericalScalar computePDF(const NumericalScalar scalar) const;
  NumericalScalar computePDF(const NumericalPoint & point) const;
  NumericalSample computePDF(const NumericalSample & sample) const;

  NumericalScalar computeLogPDF(const NumericalScalar scalar) const;
  NumericalScalar computeLogPDF(const NumericalPoint & point) const;
  NumericalSample computeLogPDF(const NumericalSample & sample) const;

  /** Compute the PDF of 1D distributions over a regular grid */
  NumericalSample computePDF(const NumericalScalar xMin,
                             const NumericalScalar xMax,
                             const UnsignedLong pointNumber,
                             const NumericalScalar precision = ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultPDFEpsilon")) const;

  /** Get the CDF of the distribution */
  NumericalScalar computeCDF(const NumericalScalar scalar, const Bool tail) const;
  NumericalScalar computeCDF(const NumericalScalar scalar) const;
  NumericalScalar computeComplementaryCDF(const NumericalScalar scalar) const;

  NumericalScalar computeCDF(const NumericalPoint & point, const Bool tail) const;
  NumericalScalar computeCDF(const NumericalPoint & point) const;
  NumericalScalar computeComplementaryCDF(const NumericalPoint & point) const;

  NumericalSample computeCDF(const NumericalSample & sample, const Bool tail) const;
  NumericalSample computeCDF(const NumericalSample & sample) const;
  NumericalSample computeComplementaryCDF(const NumericalSample & sample) const;

  /** Compute the CDF of 1D distributions over a regular grid */
  NumericalSample computeCDF(const NumericalScalar xMin,
                             const NumericalScalar xMax,
                             const UnsignedLong pointNumber,
                             const NumericalScalar precision = ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultCDFEpsilon")) const;

  NumericalSample computeComplementaryCDF(const NumericalScalar xMin,
                                          const NumericalScalar xMax,
                                          const UnsignedLong pointNumber,
                                          const NumericalScalar precision = ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultCDFEpsilon")) const;

  /** Get the probability content of an interval */
  NumericalScalar computeProbability(const Interval & interval) const;

  /** Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
  NumericalComplex computeCharacteristicFunction(const NumericalScalar x, const Bool logScale) const;
  NumericalComplex computeCharacteristicFunction(const NumericalScalar x) const;
  NumericalComplex computeLogCharacteristicFunction(const NumericalScalar x) const;

  /** Compute the generating function, i.e. psi(z) = E(z^X) */
  NumericalScalar computeGeneratingFunction(const NumericalScalar z, const Bool logScale) const;
  NumericalScalar computeGeneratingFunction(const NumericalScalar z) const;
  NumericalScalar computeLogGeneratingFunction(const NumericalScalar z) const;

  NumericalComplex computeGeneratingFunction(const NumericalComplex & z, const Bool logScale) const;
  NumericalComplex computeGeneratingFunction(const NumericalComplex & z) const;
  NumericalComplex computeLogGeneratingFunction(const NumericalComplex & z) const;

  /** Get the PDF gradient of the distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDF gradient of the distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Get the quantile of the distribution */
  NumericalPoint computeQuantile(const NumericalScalar prob,
                                 const Bool tail = false) const;
  /** Compute the quantile over a regular grid */
  NumericalSample computeQuantile(const NumericalScalar qMin,
                                  const NumericalScalar qMax,
                                  const UnsignedLong pointNumber,
                                  const NumericalScalar precision = ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon"),
                                  const Bool tail = false) const;

  /** Get the mathematical and numerical range of the distribution.
      Its mathematical range is the smallest closed interval outside
      of which the PDF is zero, and the numerical range is the interval
      outside of which the PDF is rounded to zero in double precision */
  Interval getRange() const;

  /** Get the roughness, i.e. the L2-norm of the PDF */
  NumericalScalar getRoughness() const;

  /** Get the mean of the distribution */
  NumericalPoint getMean() const;

  /** Get the covariance of the distribution */
  CovarianceMatrix getCovariance() const;

  /** Cholesky factor of the covariance matrix accessor */
  SquareMatrix getCholesky() const;

  /** Get the raw moments of the standardized distribution */
  NumericalPoint getStandardMoment(const UnsignedLong n) const;

  /** Get the raw moments of the distribution */
  NumericalPoint getMoment(const UnsignedLong n) const;

  /** Get the centered moments about the mean of the distribution */
  NumericalPoint getCenteredMoment(const UnsignedLong n) const;

  /** Inverse of the Cholesky factor of the covariance matrix accessor */
  SquareMatrix getInverseCholesky() const;

  /** Correlation matrix accessor */
  CorrelationMatrix getCorrelation() const;

  /** Get the standard deviation of the distribution */
  NumericalPoint getStandardDeviation() const;

  /** Get the skewness of the distribution */
  NumericalPoint getSkewness() const;

  /** Get the kurtosis of the distribution */
  NumericalPoint getKurtosis() const;

  /** Get the implementation of the distribution */
  Implementation getImplementation() const;

  /** Check if the distribution is elliptical */
  Bool isElliptical() const;

  /** Check if the distribution is constinuous */
  Bool isContinuous() const;

  /** Check if the distribution is discrete */
  Bool isDiscrete() const;

  /** Tell if the distribution is integer valued */
  Bool isIntegral() const;

  /** Tell if the distribution has elliptical copula */
  Bool hasEllipticalCopula() const;

  /** Tell if the distribution has independent copula */
  Bool hasIndependentCopula() const;

  /** Get the support of a distribution that intersect a given interval */
  NumericalSample getSupport(const Interval & interval) const;

  /** Get the support on the whole range */
  NumericalSample getSupport() const;

  /** Get the i-th marginal distribution */
  Distribution getMarginal(const UnsignedLong i) const;

  /** Get the distribution of the marginal distribution corresponding to indices dimensions */
  Distribution getMarginal(const Indices & indices) const;

  /** Get the copula of a distribution */
  Distribution getCopula() const;

  /** Compute the DDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
  virtual NumericalScalar computeConditionalDDF(const NumericalScalar x,
                                                const NumericalPoint & y) const;

  /** Compute the PDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
  virtual NumericalScalar computeConditionalPDF(const NumericalScalar x,
                                                const NumericalPoint & y) const;

  /** Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
  virtual NumericalScalar computeConditionalCDF(const NumericalScalar x,
                                                const NumericalPoint & y) const;

  /** Compute the quantile of Xi | X1, ..., Xi-1, i.e. x such that CDF(x|y) = q with x = Xi, y = (X1,...,Xi-1) */
  virtual NumericalScalar computeConditionalQuantile(const NumericalScalar q,
                                                     const NumericalPoint & y) const;

  /** Get the isoprobabilist transformation */
  IsoProbabilisticTransformation getIsoProbabilisticTransformation() const;

  /** Get the inverse isoprobabilist transformation */
  InverseIsoProbabilisticTransformation getInverseIsoProbabilisticTransformation() const;

  /** Get the standard distribution */
  Distribution getStandardDistribution() const;

  /** Get the standard representative in the parametric family, associated with the standard moments */
  Distribution getStandardRepresentative() const;

  /** Draw the PDF of the distribution when its dimension is 1 or 2 */
  virtual Graph drawPDF() const;

  /** Draw the PDF of the distribution when its dimension is 1 */
  virtual Graph drawPDF(const NumericalScalar xMin,
                        const NumericalScalar xMax,
                        const UnsignedLong pointNumber = ResourceMap::GetAsUnsignedLong("DistributionImplementation-DefaultPointNumber")) const;
  virtual Graph drawPDF(const UnsignedLong pointNumber) const;

  /** Draw the PDF of a 1D marginal */
  virtual Graph drawMarginal1DPDF(const UnsignedLong marginalIndex,
                                  const NumericalScalar xMin,
                                  const NumericalScalar xMax,
                                  const UnsignedLong pointNumber) const;

  /** Draw the PDF of the distribution when its dimension is 2 */
  virtual Graph drawPDF(const NumericalPoint & xMin,
                        const NumericalPoint & xMax,
                        const Indices & pointNumber) const;
  virtual Graph drawPDF(const NumericalPoint & xMin,
                        const NumericalPoint & xMax) const;
  virtual Graph drawPDF(const Indices & pointNumber) const;

  /** Draw the PDF of a 2D marginal */
  virtual Graph drawMarginal2DPDF(const UnsignedLong firstMarginal,
                                  const UnsignedLong secondMarginal,
                                  const NumericalPoint & xMin,
                                  const NumericalPoint & xMax,
                                  const Indices & pointNumber) const;

  /** Draw the CDF of the distribution when its dimension is 1 or 2 */
  virtual Graph drawCDF() const;

  /** Draw the CDF of the distribution when its dimension is 1 */
  virtual Graph drawCDF(const NumericalScalar xMin,
                        const NumericalScalar xMax,
                        const UnsignedLong pointNumber = ResourceMap::GetAsUnsignedLong("DistributionImplementation-DefaultPointNumber")) const;

  virtual Graph drawCDF(const UnsignedLong pointNumber) const;

  /** Draw the CDF of the distribution when its dimension is 2 */
  virtual Graph drawCDF(const NumericalPoint & xMin,
                        const NumericalPoint & xMax,
                        const Indices & pointNumber) const;
  virtual Graph drawCDF(const NumericalPoint & xMin,
                        const NumericalPoint & xMax) const;
  virtual Graph drawCDF(const Indices & pointNumber) const;

  /** Draw the CDF of a 1D marginal */
  virtual Graph drawMarginal1DCDF(const UnsignedLong marginalIndex,
                                  const NumericalScalar xMin,
                                  const NumericalScalar xMax,
                                  const UnsignedLong pointNumber) const;

  /** Draw the CDF of a 2D marginal */
  virtual Graph drawMarginal2DCDF(const UnsignedLong firstMarginal,
                                  const UnsignedLong secondMarginal,
                                  const NumericalPoint & xMin,
                                  const NumericalPoint & xMax,
                                  const Indices & pointNumber) const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;
  void setParametersCollection(const NumericalPointWithDescriptionCollection & parametersCollection);
  void setParametersCollection(const NumericalPointCollection & parametersCollection);
  void setParametersCollection(const NumericalPoint & parameters);

  /** Parameters number */
  UnsignedLong getParametersNumber() const;

  /** Description accessor */
  void setDescription(const Description & description);
  Description getDescription() const;

  /** Accessor to PDF computation precision */
  NumericalScalar getPDFEpsilon() const;

  /** Accessor to CDF computation precision */
  NumericalScalar getCDFEpsilon() const;

  /** Get a positon indicator for a 1D distribution */
  NumericalScalar getPositionIndicator() const;

  /** Get a dispersion indicator for a 1D distribution */
  NumericalScalar getDispersionIndicator() const;

}; /* class Distribution */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_DISTRIBUTION_HXX */
