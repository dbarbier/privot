//                                               -*- C++ -*-
/**
 *  @file  Distribution.cxx
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
 */
#include "Distribution.hxx"
#include "Uniform.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(Distribution);

/* Default constructor */
Distribution::Distribution()
  : TypedInterfaceObject<DistributionImplementation>(new Uniform())
{
  // Nothing to do
}

/* Default constructor */
Distribution::Distribution(const DistributionImplementation & implementation,
                           const String & name)
  : TypedInterfaceObject<DistributionImplementation>(implementation.clone())
{
  // Nothing to do
}

/* Constructor from implementation */
Distribution::Distribution(const Implementation & p_implementation,
                           const String & name)
  : TypedInterfaceObject<DistributionImplementation>(p_implementation)
{
  // Initialize any other class members here
  // At last, allocate memory space if needed, but go to destructor to free it
}


/* Constructor from implementation pointer */
Distribution::Distribution(DistributionImplementation * p_implementation,
                           const String & name)
  : TypedInterfaceObject<DistributionImplementation>(p_implementation)
{
  // Initialize any other class members here
  // At last, allocate memory space if needed, but go to destructor to free it
}


/* Comparison operator */
Bool Distribution::operator ==(const Distribution & other) const
{
  return (this == &other);
}

/* String converter */
String Distribution::__repr__() const
{
  return getImplementation()->__repr__();
}

/* String converter */
String Distribution::__str__(const String & offset) const
{
  return getImplementation()->__str__(offset);
}


/* Weight accessor */
void Distribution::setWeight(const NumericalScalar w)
{
  copyOnWrite();
  getImplementation()->setWeight(w);
}

/* Weight accessor */
NumericalScalar Distribution::getWeight() const
{
  return getImplementation()->getWeight();
}


/* Dimension accessor */
UnsignedLong Distribution::getDimension() const
{
  return getImplementation()->getDimension();
}

/* Get the roughness, i.e. the L2-norm of the PDF */
NumericalScalar Distribution::getRoughness() const
{
  return getImplementation()->getRoughness();
}

/* Get the mean of the distribution */
NumericalPoint Distribution::getMean() const
{
  return getImplementation()->getMean();
}

/* Get the standard deviation of the distribution */
NumericalPoint Distribution::getStandardDeviation() const
{
  return getImplementation()->getStandardDeviation();
}

/* Get the skewness of the distribution */
NumericalPoint Distribution::getSkewness() const
{
  return getImplementation()->getSkewness();
}

/* Get the kurtosis of the distribution */
NumericalPoint Distribution::getKurtosis() const
{
  return getImplementation()->getKurtosis();
}

/* Get the raw moments of the standardized distribution */
NumericalPoint Distribution::getStandardMoment(const UnsignedLong n) const
{
  return getImplementation()->getStandardMoment(n);
}

/* Get the raw moments of the distribution */
NumericalPoint Distribution::getMoment(const UnsignedLong n) const
{
  return getImplementation()->getMoment(n);
}

/* Get the centered moments about the mean of the distribution */
NumericalPoint Distribution::getCenteredMoment(const UnsignedLong n) const
{
  return getImplementation()->getCenteredMoment(n);
}

/* Get the covariance of the distribution */
CovarianceMatrix Distribution::getCovariance() const
{
  return getImplementation()->getCovariance();
}

/* Cholesky factor of the covariance matrix accessor */
SquareMatrix Distribution::getCholesky() const
{
  return getImplementation()->getCholesky();
}

/* Inverse of the Cholesky factor of the covariance matrix accessor */
SquareMatrix Distribution::getInverseCholesky() const
{
  return getImplementation()->getInverseCholesky();
}

/* Get the covariance of the distribution */
CorrelationMatrix Distribution::getCorrelation() const
{
  return getImplementation()->getCorrelation();
}


/* Get one realization of the distribution */
NumericalPoint Distribution::getRealization() const
{
  return getImplementation()->getRealization();
}

/* Get a numerical sample whose elements follow the distribution */
NumericalSample Distribution::getSample(const UnsignedLong size) const
{
  return getImplementation()->getSample(size);
}

NumericalSample Distribution::getNumericalSample(const UnsignedLong size) const
{
  return getImplementation()->getNumericalSample(size);
}

/* Get the DDF of the distribution */
NumericalPoint Distribution::computeDDF(const NumericalPoint & point) const
{
  return getImplementation()->computeDDF(point);
}

/* Get the PDF of the distribution */
NumericalScalar Distribution::computePDF(const NumericalPoint & point) const
{
  return getImplementation()->computePDF(point);
}

NumericalScalar Distribution::computeLogPDF(const NumericalPoint & point) const
{
  return getImplementation()->computeLogPDF(point);
}

/* Get the CDF of the distribution */
NumericalScalar Distribution::computeCDF(const NumericalPoint & point, const Bool tail) const
{
  return getImplementation()->computeCDF(point, tail);
}

NumericalScalar Distribution::computeCDF(const NumericalPoint & point) const
{
  return getImplementation()->computeCDF(point);
}

NumericalScalar Distribution::computeComplementaryCDF(const NumericalPoint & point) const
{
  return getImplementation()->computeComplementaryCDF(point);
}

/* Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
NumericalComplex Distribution::computeCharacteristicFunction(const NumericalScalar x,
                                                             const Bool logScale) const
{
  return getImplementation()->computeCharacteristicFunction(x, logScale);
}

NumericalComplex Distribution::computeCharacteristicFunction(const NumericalScalar x) const
{
  return getImplementation()->computeCharacteristicFunction(x);
}

NumericalComplex Distribution::computeLogCharacteristicFunction(const NumericalScalar x) const
{
  return getImplementation()->computeLogCharacteristicFunction(x);
}

/* Get the generating function of the distribution, i.e. psi(z) = E(z^X) */
NumericalScalar Distribution::computeGeneratingFunction(const NumericalScalar z,
                                                        const Bool logScale) const
{
  return getImplementation()->computeGeneratingFunction(z, logScale);
}

NumericalScalar Distribution::computeGeneratingFunction(const NumericalScalar z) const
{
  return getImplementation()->computeGeneratingFunction(z);
}

NumericalScalar Distribution::computeLogGeneratingFunction(const NumericalScalar z) const
{
  return getImplementation()->computeLogGeneratingFunction(z);
}

NumericalComplex Distribution::computeGeneratingFunction(const NumericalComplex & z,
                                                         const Bool logScale) const
{
  return getImplementation()->computeGeneratingFunction(z, logScale);
}

NumericalComplex Distribution::computeGeneratingFunction(const NumericalComplex & z) const
{
  return getImplementation()->computeGeneratingFunction(z);
}

NumericalComplex Distribution::computeLogGeneratingFunction(const NumericalComplex & z) const
{
  return getImplementation()->computeLogGeneratingFunction(z);
}

/* Get the DDF of the distribution (for a sample) */
NumericalSample Distribution::computeDDF(const NumericalSample & sample) const
{
  return getImplementation()->computeDDF(sample);
}

/* Get the PDF of the distribution (for a sample) */
NumericalSample Distribution::computePDF(const NumericalSample & sample) const
{
  return getImplementation()->computePDF(sample);
}

NumericalSample Distribution::computeLogPDF(const NumericalSample & sample) const
{
  return getImplementation()->computeLogPDF(sample);
}

/* Get the CDF of the distribution (for a sample) */
NumericalSample Distribution::computeCDF(const NumericalSample & sample, const Bool tail) const
{
  return getImplementation()->computeCDF(sample, tail);
}

NumericalSample Distribution::computeCDF(const NumericalSample & sample) const
{
  return getImplementation()->computeCDF(sample);
}

NumericalSample Distribution::computeComplementaryCDF(const NumericalSample & sample) const
{
  return getImplementation()->computeComplementaryCDF(sample);
}

/* Get the DDF of the distribution (for a scalar) */
NumericalScalar Distribution::computeDDF(const NumericalScalar scalar) const
{
  return getImplementation()->computeDDF(scalar);
}

/* Get the PDF of the distribution (for a scalar) */
NumericalScalar Distribution::computePDF(const NumericalScalar scalar) const
{
  return getImplementation()->computePDF(scalar);
}

NumericalScalar Distribution::computeLogPDF(const NumericalScalar scalar) const
{
  return getImplementation()->computeLogPDF(scalar);
}

/* Get the CDF of the distribution (for a scalar) */
NumericalScalar Distribution::computeCDF(const NumericalScalar scalar,
                                         const Bool tail) const
{
  return getImplementation()->computeCDF(scalar, tail);
}

NumericalScalar Distribution::computeCDF(const NumericalScalar scalar) const
{
  return getImplementation()->computeCDF(scalar);
}

NumericalScalar Distribution::computeComplementaryCDF(const NumericalScalar scalar) const
{
  return getImplementation()->computeComplementaryCDF(scalar);
}

/* Get the probability content of an interval */
NumericalScalar Distribution::computeProbability(const Interval & interval) const
{
  return getImplementation()->computeProbability(interval);
}

/*  Compute the PDF of 1D distributions over a regular grid */
NumericalSample Distribution::computePDF(const NumericalScalar xMin,
                                         const NumericalScalar xMax,
                                         const UnsignedLong pointNumber,
                                         const NumericalScalar precision) const
{
  return getImplementation()->computePDF(xMin, xMax, pointNumber, precision);
}

/*  Compute the CDF of 1D distributions over a regular grid */
NumericalSample Distribution::computeCDF(const NumericalScalar xMin,
                                         const NumericalScalar xMax,
                                         const UnsignedLong pointNumber,
                                         const NumericalScalar precision) const
{
  return getImplementation()->computeCDF(xMin, xMax, pointNumber, precision);
}

NumericalSample Distribution::computeComplementaryCDF(const NumericalScalar xMin,
                                                      const NumericalScalar xMax,
                                                      const UnsignedLong pointNumber,
                                                      const NumericalScalar precision) const
{
  return getImplementation()->computeComplementaryCDF(xMin, xMax, pointNumber, precision);
}

/* Get the PDF gradient of the distribution */
NumericalPoint Distribution::computePDFGradient(const NumericalPoint & point) const
{
  return getImplementation()->computePDFGradient(point);
}

/* Get the CDF gradient of the distribution */
NumericalPoint Distribution::computeCDFGradient(const NumericalPoint & point) const
{
  return getImplementation()->computeCDFGradient(point);
}



/* Get the mathematical and numerical range of the distribution.
   Its mathematical range is the smallest closed interval outside
   of which the PDF is zero, and the numerical range is the interval
   outside of which the PDF is rounded to zero in double precision */
Interval Distribution::getRange() const
{
  return getImplementation()->getRange();
}

/* Get the quantile of the distribution */
NumericalPoint Distribution::computeQuantile(const NumericalScalar prob,
                                             const Bool tail) const
{
  return getImplementation()->computeQuantile(prob, tail);
}

/* Compute the quantile over a regular grid */
NumericalSample Distribution::computeQuantile(const NumericalScalar qMin,
                                              const NumericalScalar qMax,
                                              const UnsignedLong pointNumber,
                                              const NumericalScalar precision,
                                              const Bool tail) const
{
  return getImplementation()->computeQuantile(qMin, qMax, pointNumber, precision, tail);
}

/* Get the implementation of the distribution */
Distribution::Implementation Distribution::getImplementation() const
{
  return p_implementation_;
}

/* Check if the distribution is elliptical */
Bool Distribution::isElliptical() const
{
  return getImplementation()->isElliptical();
}

/* Check if the distribution is continuous */
Bool Distribution::isContinuous() const
{
  return getImplementation()->isContinuous();
}

/* Check if the distribution is discrete */
Bool Distribution::isDiscrete() const
{
  return getImplementation()->isDiscrete();
}

/* Tell if the distribution is integer valued */
Bool Distribution::isIntegral() const
{
  return getImplementation()->isIntegral();
}

/* Tell if the distribution has elliptical copula */
Bool Distribution::hasEllipticalCopula() const
{
  return getImplementation()->hasEllipticalCopula();
}

/* Tell if the distribution has independent copula */
Bool Distribution::hasIndependentCopula() const
{
  return getImplementation()->hasIndependentCopula();
}

/* Get the support of a distribution that intersect a given interval */
NumericalSample Distribution::getSupport(const Interval & interval) const
{
  return getImplementation()->getSupport(interval);
}

/* Get the support on the whole range */
NumericalSample Distribution::getSupport() const
{
  return getImplementation()->getSupport();
}

/* Get the i-th marginal distribution */
Distribution Distribution::getMarginal(const UnsignedLong i) const
{
  return *(getImplementation()->getMarginal(i));
}

/* Get the distribution of the marginal distribution corresponding to indices dimensions */
Distribution Distribution::getMarginal(const Indices & indices) const
{
  return *(getImplementation()->getMarginal(indices));
}

/* Get the copula of a distribution */
Distribution Distribution::getCopula() const
{
  return getImplementation()->getCopula();
}

/* Compute the DDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar Distribution::computeConditionalDDF(const NumericalScalar x, const NumericalPoint & y) const
{
  return getImplementation()->computeConditionalDDF(x, y);
}

/* Compute the PDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar Distribution::computeConditionalPDF(const NumericalScalar x, const NumericalPoint & y) const
{
  return getImplementation()->computeConditionalPDF(x, y);
}

/* Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar Distribution::computeConditionalCDF(const NumericalScalar x, const NumericalPoint & y) const
{
  return getImplementation()->computeConditionalCDF(x, y);
}

/* Compute the quantile of Xi | X1, ..., Xi-1, i.e. x such that CDF(x|y) = q with x = Xi, y = (X1,...,Xi-1) */
NumericalScalar Distribution::computeConditionalQuantile(const NumericalScalar q, const NumericalPoint & y) const
{
  return getImplementation()->computeConditionalQuantile(q, y);
}

/* Get the isoprobabilist transformation */
Distribution::IsoProbabilisticTransformation Distribution::getIsoProbabilisticTransformation() const
{
  return getImplementation()->getIsoProbabilisticTransformation();
}

/* Get the inverse isoprobabilist transformation */
Distribution::InverseIsoProbabilisticTransformation Distribution::getInverseIsoProbabilisticTransformation() const
{
  return getImplementation()->getInverseIsoProbabilisticTransformation();
}

/* Get the standard distribution */
Distribution Distribution::getStandardDistribution() const
{
  return getImplementation()->getStandardDistribution();
}

/* Get the standard representative in the parametric family, associated with the standard moments */
Distribution Distribution::getStandardRepresentative() const
{
  return getImplementation()->getStandardRepresentative();
}

/* Draw the PDF of the distribution when its dimension is 1 */
Graph Distribution::drawPDF(const NumericalScalar xMin,
                            const NumericalScalar xMax,
                            const UnsignedLong pointNumber) const
{
  return getImplementation()->drawPDF(xMin, xMax, pointNumber);
}

/* Draw the PDF of the distribution when its dimension is 1 */
Graph Distribution::drawPDF(const UnsignedLong pointNumber) const
{
  return getImplementation()->drawPDF(pointNumber);
}

/* Draw the PDF of a 1D marginal */
Graph Distribution::drawMarginal1DPDF(const UnsignedLong marginalIndex,
                                      const NumericalScalar xMin,
                                      const NumericalScalar xMax,
                                      const UnsignedLong pointNumber) const
{
  return getImplementation()->drawMarginal1DPDF(marginalIndex, xMin, xMax, pointNumber);
}

/* Draw the PDF of the distribution when its dimension is 2 */
Graph Distribution::drawPDF(const NumericalPoint & xMin,
                            const NumericalPoint & xMax,
                            const Indices & pointNumber) const
{
  return getImplementation()->drawPDF(xMin, xMax, pointNumber);
}

/* Draw the PDF of the distribution when its dimension is 2 */
Graph Distribution::drawPDF(const NumericalPoint & xMin,
                            const NumericalPoint & xMax) const
{
  return getImplementation()->drawPDF(xMin, xMax);
}

/* Draw the PDF of the distribution when its dimension is 2 */
Graph Distribution::drawPDF(const Indices & pointNumber) const
{
  return getImplementation()->drawPDF(pointNumber);
}

/* Draw the PDF of a 2D marginal */
Graph Distribution::drawMarginal2DPDF(const UnsignedLong firstMarginal,
                                      const UnsignedLong secondMarginal,
                                      const NumericalPoint & xMin,
                                      const NumericalPoint & xMax,
                                      const Indices & pointNumber) const
{
  return getImplementation()->drawMarginal2DPDF(firstMarginal, secondMarginal, xMin, xMax, pointNumber);
}

/* Draw the PDF of the distribution when its dimension is 1 or 2 */
Graph Distribution::drawPDF() const
{
  return getImplementation()->drawPDF();
}

/* Draw the CDF of the distribution when its dimension is 1 */
Graph Distribution::drawCDF(const NumericalScalar xMin,
                            const NumericalScalar xMax,
                            const UnsignedLong pointNumber) const
{
  return getImplementation()->drawCDF(xMin, xMax, pointNumber);
}

/* Draw the CDF of the distribution when its dimension is 1 */
Graph Distribution::drawCDF(const UnsignedLong pointNumber) const
{
  return getImplementation()->drawCDF(pointNumber);
}

/* Draw the CDF of a 1D marginal */
Graph Distribution::drawMarginal1DCDF(const UnsignedLong marginalIndex,
                                      const NumericalScalar xMin,
                                      const NumericalScalar xMax,
                                      const UnsignedLong pointNumber) const
{
  return getImplementation()->drawMarginal1DCDF(marginalIndex, xMin, xMax, pointNumber);
}

/* Draw the CDF of the distribution when its dimension is 2 */
Graph Distribution::drawCDF(const NumericalPoint & xMin,
                            const NumericalPoint & xMax,
                            const Indices & pointNumber) const
{
  return getImplementation()->drawCDF(xMin, xMax, pointNumber);
}

/* Draw the CDF of the distribution when its dimension is 2 */
Graph Distribution::drawCDF(const NumericalPoint & xMin,
                            const NumericalPoint & xMax) const
{
  return getImplementation()->drawCDF(xMin, xMax);
}

/* Draw the CDF of the distribution when its dimension is 2 */
Graph Distribution::drawCDF(const Indices & pointNumber) const
{
  return getImplementation()->drawCDF(pointNumber);
}

/* Draw the CDF of a 2D marginal */
Graph Distribution::drawMarginal2DCDF(const UnsignedLong firstMarginal,
                                      const UnsignedLong secondMarginal,
                                      const NumericalPoint & xMin,
                                      const NumericalPoint & xMax,
                                      const Indices & pointNumber) const
{
  return getImplementation()->drawMarginal2DCDF(firstMarginal, secondMarginal, xMin, xMax, pointNumber);
}

/* Draw the CDF of the distribution when its dimension is 1 or 2 */
Graph Distribution::drawCDF() const
{
  return getImplementation()->drawCDF();
}

/* Parameters value and description accessor */
Distribution::NumericalPointWithDescriptionCollection Distribution::getParametersCollection() const
{
  return getImplementation()->getParametersCollection();
}

void Distribution::setParametersCollection(const NumericalPointWithDescriptionCollection & parametersCollection)
{
  copyOnWrite();
  getImplementation()->setParametersCollection(parametersCollection);
}

void Distribution::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  copyOnWrite();
  getImplementation()->setParametersCollection(parametersCollection);
}

void Distribution::setParametersCollection(const NumericalPoint & parameters)
{
  copyOnWrite();
  getImplementation()->setParametersCollection(parameters);
}

/* Parameters number */
UnsignedLong Distribution::getParametersNumber() const
{
  return getImplementation()->getParametersNumber();
}

/* Description accessor */
void Distribution::setDescription(const Description & description)
{
  copyOnWrite();
  getImplementation()->setDescription(description);
}

Description Distribution::getDescription() const
{
  return getImplementation()->getDescription();
}

/* Accessor to PDF computation precision */
NumericalScalar Distribution::getPDFEpsilon() const
{
  return getImplementation()->getPDFEpsilon();
}

/* Accessor to CDF computation precision */
NumericalScalar Distribution::getCDFEpsilon() const
{
  return getImplementation()->getCDFEpsilon();
}

/* Get a positon indicator for a 1D distribution */
NumericalScalar Distribution::getPositionIndicator() const
{
  return getImplementation()->getPositionIndicator();
}

/* Get a dispersion indicator for a 1D distribution */
NumericalScalar Distribution::getDispersionIndicator() const
{
  return getImplementation()->getDispersionIndicator();
}

END_NAMESPACE_OPENTURNS
