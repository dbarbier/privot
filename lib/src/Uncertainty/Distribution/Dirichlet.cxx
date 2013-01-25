//                                               -*- C++ -*-
/**
 *  @file  Dirichlet.cxx
 *  @brief The Dirichlet distribution
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
 *  @date   2010-11-21 19:45:57 +0100 (dim. 21 nov. 2010)
 */
#include <cmath>
#include "Indices.hxx"
#include "Dirichlet.hxx"
#include "RandomGenerator.hxx"
#include "SpecFunc.hxx"
#include "DistFunc.hxx"
#include "PersistentObjectFactory.hxx"
#include "ResourceMap.hxx"
#include "JacobiFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Dirichlet);

static Factory<Dirichlet> RegisteredFactory("Dirichlet");


/* Default constructor */
Dirichlet::Dirichlet()
  : NonEllipticalDistribution("Dirichlet"),
    theta_(0),
    sumTheta_(0.0),
    normalizationFactor_(0.0),
    isInitializedCDF_(false),
    integrationNodes_(0),
    integrationWeights_(0)
{
  setTheta(NumericalPoint(2, 1.0));
}

/* Parameters constructor */
Dirichlet::Dirichlet(const NumericalPoint & theta)
  : NonEllipticalDistribution("Dirichlet"),
    theta_(0),
    sumTheta_(0.0),
    normalizationFactor_(0.0),
    isInitializedCDF_(false),
    integrationNodes_(0),
    integrationWeights_(0)
{
  setTheta(theta);
}

/* Comparison operator */
Bool Dirichlet::operator ==(const Dirichlet & other) const
{
  if (this == &other) return true;
  return theta_ == other.theta_;
}

/* String converter */
String Dirichlet::__repr__() const
{
  OSS oss;
  oss << "class=" << Dirichlet::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " theta=" << theta_;
  return oss;
}

String Dirichlet::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(theta = " << theta_.__str__() << ")";
  return oss;
}

/* Virtual constructor */
Dirichlet * Dirichlet::clone() const
{
  return new Dirichlet(*this);
}

/* Compute the numerical range of the distribution given the parameters values */
void Dirichlet::computeRange()
{
  setRange(Interval(NumericalPoint(theta_.getSize() - 1, 0.0), NumericalPoint(theta_.getSize() - 1, 1.0)));
}

/* Get one realization of the distribution. We use the representation:
 * X_k = Y_k / (Y_1 + ... + Y_{d+1}) where the Y_i are independent and
 * Y_i has a Gamma(theta_i) distribution
 */
NumericalPoint Dirichlet::getRealization() const
{
  const UnsignedLong dimension(getDimension());
  if (dimension == 1) return NumericalPoint(1, DistFunc::rBeta(theta_[0], theta_[1]));
  NumericalPoint realization(dimension);
  NumericalScalar sum(DistFunc::rGamma(theta_[dimension]));
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      const NumericalScalar yI(DistFunc::rGamma(theta_[i]));
      sum += yI;
      realization[i] = yI;
    }
  return realization * (1.0 / sum);
}

/* Get the PDF of the distribution */
NumericalScalar Dirichlet::computePDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=" << dimension << ", here dimension=" << point.getDimension();

  NumericalScalar sum(0.0);
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      const NumericalScalar xI(point[i]);
      if (xI <= 0.0) return 0.0;
      sum += xI;
    }
  if (sum >= 1.0) return 0.0;
  return exp(computeLogPDF(point));
}

NumericalScalar Dirichlet::computeLogPDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=" << dimension << ", here dimension=" << point.getDimension();

  NumericalScalar sum(0.0);
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      const NumericalScalar xI(point[i]);
      if (xI <= 0.0) return -SpecFunc::MaxNumericalScalar;
      sum += xI;
    }
  if (sum >= 1.0) return -SpecFunc::MaxNumericalScalar;
  NumericalScalar logPDF(normalizationFactor_ + (theta_[dimension] - 1.0) * log1p(-sum));
  for (UnsignedLong i = 0; i < dimension; ++i) logPDF += (theta_[i] - 1.0) * log(point[i]);
  return logPDF;
}

/* Get the CDF of the distribution */
NumericalScalar Dirichlet::computeCDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=" << dimension << ", here dimension=" << point.getDimension();

  if (dimension == 1) return DistFunc::pBeta(theta_[0], theta_[1], point[0]);
  // First, the trivial cases
  Bool oneNegative(false);
  Bool allPositive(true);
  Bool allGreaterThanOne(true);
  NumericalScalar sum(0.0);
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      const NumericalScalar xI(point[i]);
      oneNegative = oneNegative || (xI <= 0.0);
      allPositive = allPositive && (xI > 0.0);
      allGreaterThanOne = allGreaterThanOne && (xI >= 1.0);
      sum += xI;
    }
  if (oneNegative) return 0.0;
  if (allGreaterThanOne) return 1.0;
  // The "inside simplex" case: use Gauss integration for now
  if (allPositive && (sum <= 1.0))
    {
      // Initialization at the first call
      static const UnsignedLong N(ResourceMap::GetAsUnsignedLong("Dirichlet-DefaultIntegrationSize"));
      // Do we have to initialize the CDF data?
      if (!isInitializedCDF_)
        {
          integrationNodes_ = NumericalPointCollection(0);
          integrationWeights_ = NumericalPointCollection(0);
          for (UnsignedLong i = 0; i < dimension; ++i)
            {
              NumericalPoint marginalWeights;
              NumericalPoint marginalNodes(JacobiFactory(0, theta_[i] - 1.0).getNodesAndWeights(N, marginalWeights));
              integrationNodes_.add(marginalNodes);
              integrationWeights_.add(marginalWeights);
            }
          isInitializedCDF_ = true;
        } // !isInitialized
      Indices indices(dimension, 0);
      NumericalScalar value(0.0);
      NumericalScalar logFactor(normalizationFactor_);
      for (UnsignedLong i = 0; i < dimension; ++i) logFactor += theta_[i] * log(point[i]) - log(theta_[i]);
      const UnsignedLong size(static_cast<UnsignedLong>(round(pow(N, dimension))));
      // Loop over the integration nodes
      for (UnsignedLong flatIndex = 0; flatIndex < size; ++flatIndex)
        {
          // The current point has components obtained by tensorization of scaled Jacobi polynomials zeros
          NumericalScalar w(1.0);
          NumericalScalar sumX(0.0);
          for (UnsignedLong i = 0; i < dimension; ++i)
            {
              const UnsignedLong indexI(indices[i]);
              const NumericalScalar lI(0.5 * point[i]);
              sumX += (integrationNodes_[i][indexI] + 1.0) * lI;
              w *= integrationWeights_[i][indexI];
            }
          const NumericalScalar dCDF(w * exp(logFactor + (theta_[dimension] - 1.0) * log1p(-sumX)));
          value += dCDF;
          // Update the indices
          ++indices[0];
          // Propagate the remainders
          for (UnsignedLong i = 0; i < dimension - 1; ++i) indices[i + 1] += (indices[i] == N);
          // Correction of the indices. The last index cannot overflow.
          for (UnsignedLong i = 0; i < dimension - 1; ++i) indices[i] = indices[i] % N;
        } // flatIndex
      return value;
    } // in the simplex
  NumericalPoint sortedPoint(point);
  std::sort(sortedPoint.begin(), sortedPoint.end());
  NumericalScalar value(0.0);
  // Can go there only if dimension > 1
  NumericalScalar sortedSum(sortedPoint[0] + sortedPoint[1]);
  if (sortedSum >= 1.0)
    {
      value = 1.0 - dimension;
      for (UnsignedLong i = 0; i < dimension; ++i) value += DistFunc::pBeta(theta_[i], sumTheta_ - theta_[i], point[i]);
      return value;
    }
  // Can go there only if dimension > 2
  sortedSum += sortedPoint[2];
  if (sortedSum >= 1.0)
    {
      value = 0.5 * (dimension - 1);
      for (UnsignedLong i = 0; i < dimension; ++i) value -= DistFunc::pBeta(theta_[i], sumTheta_ - theta_[i], point[i]);
      value *= dimension - 2;
      Indices marginal2D(2);
      NumericalPoint marginalPoint(2);
      for (UnsignedLong i = 0; i < dimension; ++i)
        {
          marginal2D[0] = i;
          marginalPoint[0] = point[i];
          for (UnsignedLong j = 0; j < i; ++j)
            {
              marginal2D[1] = j;
              marginalPoint[1] = point[j];
              value += Distribution(getMarginal(marginal2D)).computeCDF(marginalPoint);
            }
        }
      return value;
    }
  // Use crude Monte Carlo for now, with a fixed sampling size
  static const UnsignedLong samplingSize(ResourceMap::GetAsUnsignedLong("DefaultSamplingSize"));
  RandomGeneratorState initialState(RandomGenerator::GetState());
  RandomGenerator::SetSeed(samplingSize);
  static const NumericalSample sample(getSample(samplingSize));
  UnsignedLong successNumber(0);
  for (UnsignedLong i = 0; i < samplingSize; ++i)
    {
      const NumericalPoint x(sample[i]);
      Bool success(true);
      for (UnsignedLong j = 0; j < dimension; ++j)
        if (x[j] > point[j])
          {
            success = false;
            break;
          }
      successNumber += success;
    }
  RandomGenerator::SetState(initialState);
  return NumericalScalar(successNumber) / samplingSize;
}

NumericalScalar Dirichlet::computeComplementaryCDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=" << dimension << ", here dimension=" << point.getDimension();

  if (dimension == 1) return DistFunc::pBeta(theta_[0], theta_[1], point[0], true);
  // other cases do not depend on tail flag
  return computeCDF(point);
}

/* Get the quantile of the distribution */
NumericalScalar Dirichlet::computeScalarQuantile(const NumericalScalar prob,
                                                 const Bool tail,
                                                 const NumericalScalar precision) const
{
  return DistFunc::qBeta(theta_[0], theta_[1], prob, tail);
}

/* Compute the PDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar Dirichlet::computeConditionalPDF(const NumericalScalar x,
                                                 const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional PDF with a conditioning point of dimension greater or equal to the distribution dimension.";
  NumericalScalar sum(0.0);
  for (UnsignedLong i = 0; i < conditioningDimension; ++i) sum += theta_[i];
  const NumericalScalar r(theta_[conditioningDimension]);
  const NumericalScalar s(sumTheta_ - sum - r);
  const NumericalScalar z(x / (1.0 - sum));
  return exp(- SpecFunc::LnBeta(r, s) + (r - 1.0) * log(z) + (s - 1.0) * log1p(-z)) / (1.0 - sum);
}

/* Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar Dirichlet::computeConditionalCDF(const NumericalScalar x,
                                                 const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional CDF with a conditioning point of dimension greater or equal to the distribution dimension.";
  NumericalScalar sum(0.0);
  for (UnsignedLong i = 0; i < conditioningDimension; ++i) sum += theta_[i];
  const NumericalScalar r(theta_[conditioningDimension]);
  const NumericalScalar s(sumTheta_ - sum - r);
  return DistFunc::pBeta(r, s, x / (1.0 - sum));
}

/* Compute the quantile of Xi | X1, ..., Xi-1, i.e. x such that CDF(x|y) = q with x = Xi, y = (X1,...,Xi-1) */
NumericalScalar Dirichlet::computeConditionalQuantile(const NumericalScalar q,
                                                      const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile with a conditioning point of dimension greater or equal to the distribution dimension.";
  if ((q < 0.0) || (q > 1.0)) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile for a probability level outside of [0, 1]";
  NumericalScalar sum(0.0);
  for (UnsignedLong i = 0; i < conditioningDimension; ++i) sum += theta_[i];
  const NumericalScalar r(theta_[conditioningDimension]);
  const NumericalScalar s(sumTheta_ - sum - r);
  return (1.0 - sum) * DistFunc::qBeta(r, s, q);
}

/* Compute the mean of the distribution */
void Dirichlet::computeMean() const
{
  const UnsignedLong dimension(getDimension());
  mean_ = NumericalPoint(dimension);
  for (UnsignedLong i = 0; i < dimension; ++i) mean_[i] = theta_[i] / sumTheta_;
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint Dirichlet::getStandardDeviation() const
{
  const UnsignedLong dimension(getDimension());
  NumericalPoint sigma(dimension);
  NumericalScalar factor(1.0 / (sumTheta_ * sqrt(1.0 + sumTheta_)));
  for (UnsignedLong i = 0; i < dimension; ++i) sigma[i] = theta_[i] * (sumTheta_ - theta_[i]) * factor;
  return sigma;
}

/* Get the skewness of the distribution */
NumericalPoint Dirichlet::getSkewness() const
{
  const UnsignedLong dimension(getDimension());
  NumericalPoint skewness(dimension);
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      const NumericalScalar thetaI(theta_[i]);
      skewness[i] = 2.0 * (sumTheta_ - 2.0 * thetaI) / (sumTheta_ + 2.0) * sqrt(sumTheta_ + 1.0) / (thetaI * (sumTheta_ - thetaI));
    }
  return skewness;
}

/* Get the kurtosis of the distribution */
NumericalPoint Dirichlet::getKurtosis() const
{
  const UnsignedLong dimension(getDimension());
  NumericalPoint kurtosis(dimension);
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      const NumericalScalar thetaI(theta_[i]);
      kurtosis[i] = 3.0 * (sumTheta_ + 1.0) * (2.0 * sumTheta_ * sumTheta_ + thetaI * (sumTheta_ - 6.0) * (sumTheta_ - thetaI)) / (thetaI * (sumTheta_ - thetaI) * (3.0 + sumTheta_) * (2.0 + sumTheta_));
    }
  return kurtosis;
}

/* Compute the covariance of the distribution */
void Dirichlet::computeCovariance() const
{
  const UnsignedLong dimension(getDimension());
  covariance_ = CovarianceMatrix(dimension);
  NumericalScalar factor(1.0 / (sumTheta_ * sumTheta_ * (1.0 + sumTheta_)));
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      const NumericalScalar thetaI(theta_[i]);
      covariance_(i, i) = factor * (thetaI * (sumTheta_ - thetaI));
      for (UnsignedLong j = 0; j < i; ++j) covariance_(i, j) = -factor * thetaI * theta_[j];
    }
  isAlreadyComputedCovariance_ = true;
}

/* Theta accessor */
void Dirichlet::setTheta(const NumericalPoint & theta)
{
  const UnsignedLong size(theta.getSize());
  if (size <= 1) throw InvalidArgumentException(HERE) << "Error: the parameter theta must be of size at least 2.";
  for (UnsignedLong i = 0; i < size; ++i) if (theta[i] <= 0.0) throw InvalidArgumentException(HERE) << "Error: all the components of theta must be positive";
  theta_ = theta;
  sumTheta_ = 0.0;
  normalizationFactor_ = 0.0;
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalScalar thetaI(theta[i]);
      if (thetaI <= 0.0) throw InvalidArgumentException(HERE) << "Error: the vector theta must have positive components, here theta(" << i << ")=" << thetaI;
      normalizationFactor_ -= SpecFunc::LnGamma(thetaI);
      sumTheta_ += thetaI;
    }
  normalizationFactor_ += SpecFunc::LnGamma(sumTheta_);
  setDimension(size - 1);
  isAlreadyComputedMean_ = false;
  isAlreadyComputedCovariance_ = false;
}

NumericalPoint Dirichlet::getTheta() const
{
  return theta_;
}

/* Get the i-th marginal distribution */
Dirichlet::Implementation Dirichlet::getMarginal(const UnsignedLong i) const
{
  const UnsignedLong dimension(getDimension());
  if (i >= dimension) throw InvalidArgumentException(HERE) << "The index of a marginal distribution must be in the range [0, dim-1]";
  if (dimension == 1) return clone();
  NumericalPoint thetaMarginal(2);
  thetaMarginal[0] = theta_[i];
  thetaMarginal[1] = sumTheta_ - theta_[i];
  return new Dirichlet(thetaMarginal);
}

/* Get the distribution of the marginal distribution corresponding to indices dimensions */
Dirichlet::Implementation Dirichlet::getMarginal(const Indices & indices) const
{
  const UnsignedLong dimension(getDimension());
  if (!indices.check(dimension - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal distribution must be in the range [0, dim-1] and  must be different";
  if (dimension == 1) return clone();
  const UnsignedLong outputDimension(indices.getSize());
  NumericalPoint thetaMarginal(outputDimension + 1);
  NumericalScalar sumMarginal(0.0);
  for (UnsignedLong i = 0; i < outputDimension; ++i)
    {
      const NumericalScalar thetaI(theta_[indices[i]]);
      sumMarginal += thetaI;
      thetaMarginal[i] = thetaI;
    }
  thetaMarginal[outputDimension] = sumTheta_ - sumMarginal;
  Dirichlet marginal(thetaMarginal);
  // Initialize the CDF computation if the data are available
  if (isInitializedCDF_)
    {
      NumericalPointCollection marginalIntegrationNodes_(0);
      NumericalPointCollection marginalIntegrationWeights_(0);
      for (UnsignedLong i = 0; i < outputDimension; ++i)
        {
          marginalIntegrationNodes_.add(integrationNodes_[indices[i]]);
          marginalIntegrationWeights_.add(integrationWeights_[indices[i]]);
        }
      marginal.integrationNodes_ = marginalIntegrationNodes_;
      marginal.integrationWeights_ = marginalIntegrationWeights_;
      marginal.isInitializedCDF_ = true;
    }
  return marginal.clone();
} // getMarginal(Indices)


DistributionImplementation::NumericalPointWithDescriptionCollection Dirichlet::getParametersCollection() const
{
  const UnsignedLong dimension(getDimension());
  NumericalPointWithDescriptionCollection parameters(dimension);
  const Description description(getDescription());
  for (UnsignedLong marginalIndex = 0; marginalIndex < dimension; ++marginalIndex)
    {
      NumericalPointWithDescription point(2);
      Description marginalDescription(point.getDimension());
      point[0] = theta_[marginalIndex];
      point[1] = sumTheta_ - theta_[marginalIndex];
      marginalDescription[0] = "theta";
      marginalDescription[1] = "sum theta";
      point.setDescription(marginalDescription);
      point.setName(description[marginalIndex]);
      parameters[marginalIndex] = point;
    } // marginalIndex
  return parameters;
}


void Dirichlet::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  const UnsignedLong size(parametersCollection.getSize());
  const UnsignedLong dimension(size);
  NumericalPoint theta(dimension + 1);
  if ( size == 0 ) throw InvalidArgumentException(HERE) << "The collection is empty.";
  if ( parametersCollection[0].getSize() < 2 ) throw InvalidArgumentException(HERE) << "The collection is too small.";
  NumericalScalar lastTheta = parametersCollection[0][0] + parametersCollection[0][1];// sum of all thetas
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      theta[i] = parametersCollection[i][0];
      lastTheta -= theta[i]; // substract each theta except the last one
    }
  theta[dimension] = lastTheta;
  setTheta(theta);
}


/* Method save() stores the object through the StorageManager */
void Dirichlet::save(Advocate & adv) const
{
  NonEllipticalDistribution::save(adv);
  adv.saveAttribute( "theta_", theta_ );
  adv.saveAttribute( "sumTheta_", sumTheta_ );
  adv.saveAttribute( "normalizationFactor_", normalizationFactor_ );
}

/* Method load() reloads the object from the StorageManager */
void Dirichlet::load(Advocate & adv)
{
  NonEllipticalDistribution::load(adv);
  adv.loadAttribute( "theta_", theta_ );
  adv.loadAttribute( "sumTheta_", sumTheta_ );
  adv.loadAttribute( "normalizationFactor_", normalizationFactor_ );
  computeRange();
}

END_NAMESPACE_OPENTURNS
