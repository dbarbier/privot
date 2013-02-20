//                                               -*- C++ -*-
/**
 *  @file  KernelMixture.cxx
 *  @brief Class for a product-kernel multidimensional mixture. If K is the
 *         underlying 1D kernel, h=(h_1,\\dot,h_n) is the vector of bandwidth
 *         and X=(X^1,\\dots,X^N) is the nD sample, the PDF of the kernel mixture
 *         is:
 *         PDF(x) = C\\sum_{i=1}^N\\prod_{j=1}^n K((x_j - X^i_j)/h_j),
 *         where:
 *         C = \\frac{1}{N\\prod_{k=1}^n h_k}
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
#include <cmath>
#include "KernelMixture.hxx"
#include "RandomGenerator.hxx"
#include "Exception.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(KernelMixture);

static Factory<KernelMixture> RegisteredFactory("KernelMixture");

/* Default constructor */
KernelMixture::KernelMixture(const Distribution & kernel,
                             const NumericalPoint & bandwidth,
                             const NumericalSample & sample)
  : NonEllipticalDistribution("KernelMixture"),
    kernel_(kernel),
    bandwidth_(0),
    bandwidthInverse_(0),
    normalizationFactor_(0.0),
    sample_(sample)
{
  // We check if the given kernel is 1-D (product kernel)
  if (kernel.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: only 1D kernel is allowed for multidimensional product kernels";
  if (sample.getSize() == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a KernelMixture based on an empty sample.";
  setDimension(sample.getDimension());
  // This call set also the range.
  setBandwidth(bandwidth);
}

/* Comparison operator */
Bool KernelMixture::operator ==(const KernelMixture & other) const
{
  if (this == &other) return true;
  return (bandwidth_ == other.bandwidth_) && (kernel_ == other.kernel_) && (sample_ == other.sample_);
}

/* String converter */
String KernelMixture::__repr__() const
{
  OSS oss;
  oss << "class=" << KernelMixture::GetClassName()
      << " name=" << getName()
      << " kernel=" << kernel_
      << " bandwidth=" << bandwidth_
      << " sample=" << sample_;
  return oss;
}

String KernelMixture::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(kernel = " << kernel_.__str__() << ", bandwidth = " << bandwidth_.__str__() << ", sample = " << "\n" << sample_.__str__(offset);
  return oss;
}


/* Compute the numerical range of the distribution given the parameters values */
void KernelMixture::computeRange()
{
  const Interval kernelRange(kernel_.getRange());
  const UnsignedLong dimension(getDimension());
  const NumericalPoint lowerBound(sample_.getMin() + kernelRange.getLowerBound()[0] * bandwidth_);
  const NumericalPoint upperBound(sample_.getMax() + kernelRange.getUpperBound()[0] * bandwidth_);
  const Interval::BoolCollection finiteLowerBound(dimension, kernelRange.getFiniteLowerBound()[0]);
  const Interval::BoolCollection finiteUpperBound(dimension, kernelRange.getFiniteUpperBound()[0]);
  setRange(Interval(lowerBound, upperBound, finiteLowerBound, finiteUpperBound));
}

/* Kernel accessor */
void KernelMixture::setKernel(const Distribution & kernel)
{
  // We check if the kernel is 1D
  if (kernel.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the kernel must be 1D for product kernel mixture";
  kernel_ = kernel;
  isAlreadyComputedMean_ = false;
  isAlreadyComputedCovariance_ = false;
  computeRange();
}

Distribution KernelMixture::getKernel() const
{
  return kernel_;
}

/* Sample accessor */
void KernelMixture::setInternalSample(const NumericalSample & sample)
{
  if (sample.getSize() == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a KernelMixture based on an empty sample.";
  sample_ = sample;
  isAlreadyComputedMean_ = false;
  isAlreadyComputedCovariance_ = false;
  computeRange();
}

NumericalSample KernelMixture::getInternalSample() const
{
  return sample_;
}


/* Bandwidth accessor */
void KernelMixture::setBandwidth(const NumericalPoint & bandwidth)
{
  const UnsignedLong dimension(getDimension());
  normalizationFactor_ = sample_.getSize();
  if (bandwidth.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the dimensions of the bandwidth and the sample must be equal";
  bandwidthInverse_ = NumericalPoint(dimension);
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      const NumericalScalar hi(bandwidth[i]);
      if (hi <= 0.0) throw InvalidArgumentException(HERE) << "Error: the bandwidth components must be > 0, here bandwidth=" << bandwidth;
      bandwidthInverse_[i] = 1.0 / hi;
      normalizationFactor_ *= hi;
    }
  bandwidth_ = bandwidth;
  normalizationFactor_ = 1.0 / normalizationFactor_;
  isAlreadyComputedMean_ = false;
  isAlreadyComputedCovariance_ = false;
  computeRange();
}

/* Distribution collection accessor */
NumericalPoint KernelMixture::getBandwidth() const
{
  return bandwidth_;
}

/* Virtual constructor */
KernelMixture * KernelMixture::clone() const
{
  return new KernelMixture(*this);
}

/* Get one realization of the KernelMixture */
NumericalPoint KernelMixture::getRealization() const
{
  // Select the atom uniformly amongst the possible points
  NumericalPoint result(sample_[RandomGenerator::IntegerGenerate(sample_.getSize())]);
  // Then add a random noise according to the product kernel
  const UnsignedLong dimension(getDimension());
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      result[i] += bandwidth_[i] * kernel_.getRealization()[0];
    }
  return result;
}

/* Get the DDF of the KernelMixture */
NumericalPoint KernelMixture::computeDDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  NumericalPoint ddfValue(dimension, 0.0);
  // Quick rejection test
  if (!getRange().numericallyContains(point)) return ddfValue;
  const UnsignedLong size(sample_.getSize());
  for(UnsignedLong i = 0; i < size; ++i)
    {
      NumericalPoint atom(dimension, 0.0);
      NumericalPoint kernelPdfAtom(dimension, 0.0);
      NumericalScalar pdfAtom(1.0);
      for (UnsignedLong j = 0; j < dimension; ++j)
        {
          atom[j] = (point[j] - sample_[i][j]) * bandwidthInverse_[j];
          kernelPdfAtom[j] = kernel_.computePDF(NumericalPoint(1, atom[j]));
          pdfAtom *= kernelPdfAtom[j];
        }
      for (UnsignedLong j = 0; j < dimension; ++j)
        {
          // Only aggregate the values associated with kernelPdfAtom>0
          if (kernelPdfAtom[j] > 0.0)
            ddfValue[j] += pdfAtom / kernelPdfAtom[j] * kernel_.computeDDF(NumericalPoint(1, atom[j]))[0] * bandwidthInverse_[j];
        }
    } /* end for */
  return normalizationFactor_ * ddfValue;
}

/* Get the PDF of the KernelMixture */
NumericalScalar KernelMixture::computePDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  NumericalScalar pdfValue(0.0);
  // Quick rejection test
  if (!getRange().numericallyContains(point)) return pdfValue;
  const UnsignedLong size(sample_.getSize());
  const NumericalScalar pdfEpsilon(ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultPDFEpsilon"));
  for(UnsignedLong i = 0; i < size; ++i)
    {
      NumericalScalar pdfAtom(1.0);
      for (UnsignedLong j = 0; j < dimension; ++j)
        {
          pdfAtom *= kernel_.computePDF(NumericalPoint(1, (point[j] - sample_[i][j]) * bandwidthInverse_[j]));
          if (pdfAtom < pdfEpsilon) break;
        }
      pdfValue += pdfAtom;
    } /* end for */
  return normalizationFactor_ * pdfValue;
}

/* Get the CDF of the KernelMixture */
NumericalScalar KernelMixture::computeCDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  NumericalScalar cdfValue(0.0);
  const UnsignedLong size(sample_.getSize());
  for(UnsignedLong i = 0; i < size; ++i)
    {
      NumericalScalar cdfAtom(1.0);
      for (UnsignedLong j = 0; j < dimension; ++j)
        cdfAtom *= kernel_.computeCDF((point[j] - sample_[i][j]) * bandwidthInverse_[j]);
      cdfValue += cdfAtom;
    } /* end for */
  cdfEpsilon_ = kernel_.getCDFEpsilon() * size;
  return cdfValue / size;
}

/* Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
NumericalComplex KernelMixture::computeCharacteristicFunction(const NumericalScalar x) const
{
  if (x == 0.0) return 1.0;
  NumericalComplex cfValue(0.0);
  const UnsignedLong size(sample_.getSize());
  for(UnsignedLong i = 0; i < size; ++i)
    {
      cfValue += kernel_.computeCharacteristicFunction(x * bandwidth_[0]) * exp(NumericalComplex(0.0, sample_[i][0] * x));
    } /* end for */
  return cfValue * (1.0 / size);
}

/* Get the PDF gradient of the distribution */
NumericalPoint KernelMixture::computePDFGradient(const NumericalPoint & point) const
{
  throw NotYetImplementedException(HERE);
}

/* Get the CDF gradient of the distribution */
NumericalPoint KernelMixture::computeCDFGradient(const NumericalPoint & point) const
{
  throw NotYetImplementedException(HERE);
}

/* Get the i-th marginal distribution */
KernelMixture::Implementation KernelMixture::getMarginal(const UnsignedLong i) const
{
  const UnsignedLong dimension(getDimension());
  if (i >= dimension) throw InvalidArgumentException(HERE) << "The index of a marginal distribution must be in the range [0, dim-1]";
  // Special case for dimension 1
  if (dimension == 1) return clone();
  // General case
  return new KernelMixture(kernel_, NumericalPoint(1, bandwidth_[i]), sample_.getMarginal(i));
}

/* Get the distribution of the marginal distribution corresponding to indices dimensions */
KernelMixture::Implementation KernelMixture::getMarginal(const Indices & indices) const
{
  const UnsignedLong dimension(getDimension());
  if (!indices.check(dimension - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal distribution must be in the range [0, dim-1] and  must be different";
  // Special case for dimension 1
  if (dimension == 1) return clone();
  // General case
  const UnsignedLong marginalDimension(indices.getSize());
  NumericalPoint marginalBandwidth(marginalDimension);
  for (UnsignedLong i = 0; i < marginalDimension; ++i)
    {
      marginalBandwidth[i] = bandwidth_[indices[i]];
    }
  return new KernelMixture(kernel_, marginalBandwidth, sample_.getMarginal(indices));
}

/* Compute the mean of the KernelMixture
 * PDF(x) = C\sum_{i=1}^N\prod_{j=1}^n K((X^i_j-x_j)/h_j),
 * where:
 * C = \frac{1}{N\prod_{k=1}^n h_k}
 * mu_j = \int_{-\infty}^{\infty} x_j\frac{1}{Nh_j}\sum_{i=1}^N K((x_j - X^i_j) / h_j) dx_j
 = \int_{-\infty}^{\infty} \frac{1}{Nh_j}\sum_{i=1}^N(h_jt + X^i_j)K(t) h_j dt
 = \frac{1}{Nh_j}\sum_{i=1}^NX^i_j + \int_{-\infty}^{\infty} \frac{1}{N}\sum_{i=1}^Nh_jtK(t) dt
 = mu_sample_j + h_j mu_K
*/
void KernelMixture::computeMean() const
{
  // We know that the kernel is 1D, so its mean value is actually a scalar
  const NumericalScalar meanKernel(kernel_.getMean()[0]);
  mean_ = sample_.computeMean();
  // Special case for symmetric kernel
  if (meanKernel == 0.0) return;
  // General case
  mean_ += meanKernel * bandwidth_;
  isAlreadyComputedMean_ = true;
}

/* Compute the covariance of the KernelMixture
   Covariance(KernelMixture) = (1-1/N) Covariance(sample) + Covariance(kernel) * diag(bandwidth[i]^2)
*/
void KernelMixture::computeCovariance() const
{
  const UnsignedLong dimension(getDimension());
  // We know that the kernel is 1D, so its standard deviation is actually a scalar
  const NumericalScalar sigmaKernel(kernel_.getStandardDeviation()[0]);
  // Covariance(sample) term, with the proper scaling
  covariance_ = CovarianceMatrix(dimension, Collection<NumericalScalar>(sample_.computeCovariance().getImplementation()->operator*(1.0 - 1.0 / sample_.getSize())));
  // Add the diagonal kernel covariance contribution
  for (UnsignedLong i = 0; i < dimension; ++i)
    covariance_(i, i) += pow(bandwidth_[i] * sigmaKernel, 2);
  isAlreadyComputedCovariance_ = true;
}

/* Get the standard deviation of the distribution. We don't use the square root of the covariance since it involves a O(dim^2) computation where only a O(dim) computation is required.
   std = [var_sample + h^2 var_K]^(1/2)
*/
NumericalPoint KernelMixture::getStandardDeviation() const
{
  const UnsignedLong dimension(getDimension());
  // We know that the kernel is 1D, so its standard deviation is actually a scalar
  const NumericalScalar sigmaKernel(kernel_.getStandardDeviation()[0]);
  NumericalPoint result(sample_.computeCenteredMomentPerComponent(2));
  for (UnsignedLong i = 0; i < dimension; ++i)
    result[i] = sqrt(result[i] + pow(bandwidth_[i] * sigmaKernel, 2));
  return result;
}

/* Get the skewness of the distribution:
   skew = [skew_sample * std_sample^3 + h^3 * skew_K * std_K^3] / std^3
*/
NumericalPoint KernelMixture::getSkewness() const
{
  const UnsignedLong dimension(getDimension());
  // We know that the kernel is 1D, so its standard deviation is actually a scalar
  const NumericalScalar sigmaKernel(kernel_.getStandardDeviation()[0]);
  // We know that the kernel is 1D, so its skewness is actually a scalar
  const NumericalScalar skewnessKernel(kernel_.getSkewness()[0]);
  // Standard deviation of the KernelMixture
  const NumericalPoint sigma(getStandardDeviation());
  NumericalPoint result(sample_.computeCenteredMomentPerComponent(3));
  for (UnsignedLong i = 0; i < dimension; ++i)
    result[i] = (result[i] + pow(bandwidth_[i] * sigmaKernel, 3) * skewnessKernel) / pow(sigma[i], 3);
  return result;
}

/* Get the kurtosis of the distribution:
   kurt = [kurt_sample * std_sample^4 + h^4 * kurt_K * std_K^4 + 6 * h^2 * var_sample * var_K] / std^4
*/
NumericalPoint KernelMixture::getKurtosis() const
{
  const UnsignedLong dimension(getDimension());
  // We know that the kernel is 1D, so its standard deviation is actually a scalar
  const NumericalScalar sigmaKernel(kernel_.getStandardDeviation()[0]);
  // We know that the kernel is 1D, so its skewness is actually a scalar
  const NumericalScalar kurtosisKernel(kernel_.getKurtosis()[0]);
  // Standard deviation of the sample
  const NumericalPoint varSample(sample_.computeCenteredMomentPerComponent(2));
  // Standard deviation of the KernelMixture
  const NumericalPoint sigma(getStandardDeviation());
  NumericalPoint result(sample_.computeCenteredMomentPerComponent(4));
  for (UnsignedLong i = 0; i < dimension; ++i)
    result[i] = (result[i] + pow(bandwidth_[i] * sigmaKernel, 4) * kurtosisKernel + 6.0 * varSample[i] * pow(bandwidth_[i] * sigmaKernel, 2)) / pow(sigma[i], 4);
  return result;
}

/* Parameters value and description accessor */
KernelMixture::NumericalPointWithDescriptionCollection KernelMixture::getParametersCollection() const
{
  const UnsignedLong dimension(getDimension());
  const UnsignedLong size(sample_.getSize());
  NumericalPointWithDescriptionCollection parameters(dimension + 1);
  // The marginal parameters : the sample and the bandwidth
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      NumericalPointWithDescription marginalParameters(size + 1);
      for (UnsignedLong j = 0; j < size; ++j)
        marginalParameters[j] = sample_[j][i];
      marginalParameters[size] = bandwidth_[i];
      parameters[i] = marginalParameters;
    }
  parameters[dimension] = kernel_.getParametersCollection()[0];
  return parameters;
} // getParametersCollection

/* Check if the distribution is elliptical */
Bool KernelMixture::isElliptical() const
{
  if ((sample_.getSize() == 1) && (getDimension() == 1)) return kernel_.getImplementation()->isElliptical();
  return false;
}

/* Check if the distribution is continuos */
Bool KernelMixture::isContinuous() const
{
  return kernel_.getImplementation()->isContinuous();
}

/* Tell if the distribution has elliptical copula */
Bool KernelMixture::hasEllipticalCopula() const
{
  // In 1D, all the distributions have an elliptical copula
  if (getDimension() == 1) return true;
  return false;
}

/* Tell if the distribution has independent copula */
Bool KernelMixture::hasIndependentCopula() const
{
  // In 1D, all the distributions have an independent copula
  return (getDimension() == 1);
}

/* Method save() stores the object through the StorageManager */
void KernelMixture::save(Advocate & adv) const
{
  NonEllipticalDistribution::save(adv);
  adv.saveAttribute( "kernel_", kernel_ );
  adv.saveAttribute( "bandwidth_", bandwidth_ );
  adv.saveAttribute( "bandwidthInverse_", bandwidthInverse_ );
  adv.saveAttribute( "normalizationFactor_", normalizationFactor_ );
  adv.saveAttribute( "sample_", sample_ );
}

/* Method load() reloads the object from the StorageManager */
void KernelMixture::load(Advocate & adv)
{
  NonEllipticalDistribution::load(adv);
  adv.loadAttribute( "kernel_", kernel_ );
  adv.loadAttribute( "bandwidth_", bandwidth_ );
  adv.loadAttribute( "bandwidthInverse_", bandwidthInverse_ );
  adv.loadAttribute( "normalizationFactor_", normalizationFactor_ );
  adv.loadAttribute( "sample_", sample_ );
  computeRange();
}

END_NAMESPACE_OPENTURNS
