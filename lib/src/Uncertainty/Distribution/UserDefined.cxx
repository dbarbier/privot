//                                               -*- C++ -*-
/**
 *  @file  UserDefined.cxx
 *  @brief The UserDefined distribution
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */
#include <cstdlib>
#include <cmath>
#include "UserDefined.hxx"
#include "RandomGenerator.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

TEMPLATE_CLASSNAMEINIT(PersistentCollection<UserDefinedPair>);
static Factory<PersistentCollection<UserDefinedPair> > RegisteredFactory("PersistentCollection<UserDefinedPair>");

CLASSNAMEINIT(UserDefined);
static Factory<UserDefined> RegisteredFactory_alt2("UserDefined");


/* Default constructor */
UserDefined::UserDefined()
  : DiscreteDistribution("UserDefined")
  , collection_(UserDefinedPairCollection(1))
  , cumulativeProbabilities_(NumericalScalarCollection(1, 1.0))
  , hasUniformWeights_(true)
{
  // Empty range
  setRange(Interval(1.0, 0.0));
}

/* Constructor from UserDefinedPairCollection */
UserDefined::UserDefined(const UserDefinedPairCollection & collection)
  : DiscreteDistribution("UserDefined")
  , collection_(0)
  , cumulativeProbabilities_(0)
  , hasUniformWeights_(false)
{
  // We set the dimension of the UserDefined distribution
  // This call set also the range
  setPairCollection( collection );
}

/* Constructor from a sample */
UserDefined::UserDefined(const NumericalSample & sample)
  : DiscreteDistribution("UserDefined")
  , collection_(0)
  , cumulativeProbabilities_(0)
  , hasUniformWeights_(true)
{
  const UnsignedLong size(sample.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a UserDefined distribution based on an empty sample.";
  UserDefinedPairCollection collection(size);
  const NumericalScalar w(1.0 / size);
  for (UnsignedLong i = 0; i < size; ++i) collection[i] = UserDefinedPair(sample[i], w);
  // We set the dimension of the UserDefined distribution
  // This call set also the range
  setPairCollection( collection );
  setName(sample.getName());
}

/* Constructor from a sample and the associated weights */
UserDefined::UserDefined(const NumericalSample & sample,
                         const NumericalPoint & weights)
  : DiscreteDistribution("UserDefined")
  , collection_(0)
  , cumulativeProbabilities_(0)
  , hasUniformWeights_(false)
{
  const UnsignedLong size(sample.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a UserDefined distribution based on an empty sample.";
  if (weights.getDimension() != size) throw InvalidArgumentException(HERE) << "Error: cannot build a UserDefined distribution if the weights don't have the same dimension as the sample size.";
  UserDefinedPairCollection collection(size);
  // Normalize the weights here
  NumericalScalar sumWeights(0.0);
  for (UnsignedLong i = 0; i < size; ++i) sumWeights += weights[i];
  for (UnsignedLong i = 0; i < size; ++i) collection[i] = UserDefinedPair(sample[i], weights[i] / sumWeights);
  // We set the dimension of the UserDefined distribution
  // This call set also the range
  setPairCollection( collection );
  setName(sample.getName());
}

/* Comparison operator */
Bool UserDefined::operator ==(const UserDefined & other) const
{
  if (this == &other) return true;
  return collection_ == other.collection_;
}

/* String converter */
String UserDefined::__repr__() const
{
  OSS oss;
  oss << "class=" << UserDefined::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " collection=" << collection_;
  return oss;
}

String UserDefined::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(";
  String separator("");
  for (UnsignedLong i = 0; i < collection_.getSize(); ++i)
    {
      oss << separator << "{x = " << collection_[i].getX().__str__() << ", p = " << collection_[i].getP() << "}";
      separator = ", ";
    }
  oss << ")";
  return oss;
}

/* Virtual constructor */
UserDefined * UserDefined::clone() const
{
  return new UserDefined(*this);
}

/* Get one realization of the distribution */
NumericalPoint UserDefined::getRealization() const
{
  // Efficient algorithm for uniform weights
  const UnsignedLong size(collection_.getSize());
  if (hasUniformWeights_)
    {
      const UnsignedLong j(RandomGenerator::IntegerGenerate(size));
      return collection_[j].getX();
    }
  const NumericalScalar uniformRealization(RandomGenerator::Generate());
  if (uniformRealization <= cumulativeProbabilities_[0]) return collection_[0].getX();
  UnsignedLong j0(0);
  UnsignedLong j1(size - 1);
  while (j1 - j0 > 1)
    {
      const UnsignedLong jm((j0 + j1) / 2);
      if (uniformRealization > cumulativeProbabilities_[jm]) j0 = jm;
      else j1 = jm;
    }
  return collection_[j1].getX();
}

/* Get the PDF of the distribution */
NumericalScalar UserDefined::computePDF(const NumericalPoint & point) const
{
  const UnsignedLong size(collection_.getSize());
  const UnsignedLong dimension(getDimension());
  NumericalScalar pdf(0.0);
  // Quick search for 1D case
  if (dimension == 1)
    {
      const NumericalScalar x(point[0]);
      UnsignedLong upper(size - 1);
      NumericalScalar xUpper(collection_[upper].getX()[0]);
      if (x > xUpper + ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) return 0.0;
      UnsignedLong lower(0);
      NumericalScalar xLower(collection_[lower].getX()[0]);
      if (x < xLower - ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) return 0.0;
      // Use dichotomic search of the correct index
      while (upper - lower > 1)
        {
          // The integer arithmetic insure that middle will be strictly between lower and upper as far as upper - lower > 1
          const UnsignedLong middle((upper + lower) / 2);
          const NumericalScalar xMiddle(collection_[middle].getX()[0]);
          if (xMiddle > x + ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon"))
            {
              upper = middle;
              xUpper = xMiddle;
            }
          else
            {
              lower = middle;
              xLower = xMiddle;
            }
        } // while
      // At this point we have upper == lower or upper == lower + 1, with lower - epsilon <= x < upper + epsilon
      if (fabs(x - xUpper) <= ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) return collection_[upper].getP();
      if (fabs(x - xLower) <= ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) return collection_[lower].getP();
      // x is not a point in the support of the distribution
      return 0.0;
    }
  for (UnsignedLong i = 0; i < size; ++i) if ((point - collection_[i].getX()).norm() < ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) pdf += collection_[i].getP();
  return pdf;
}

/* Get the CDF of the distribution */
NumericalScalar UserDefined::computeCDF(const NumericalPoint & point) const
{
  NumericalScalar cdf(0.0);
  const UnsignedLong size(collection_.getSize());
  const UnsignedLong dimension(getDimension());
  // Quick search for 1D case
  if (dimension == 1)
    {
      const NumericalScalar x(point[0]);
      UnsignedLong upper(size - 1);
      NumericalScalar xUpper(collection_[upper].getX()[0]);
      if (x > xUpper - ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) return 1.0;
      UnsignedLong lower(0);
      NumericalScalar xLower(collection_[lower].getX()[0]);
      if (x <= xLower - ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) return 0.0;
      // Use dichotomic search of the correct index
      while (upper - lower > 1)
        {
          // The integer arithmetic insure that middle will be strictly between lower and upper as far as upper - lower > 1
          const UnsignedLong middle((upper + lower) / 2);
          const NumericalScalar xMiddle(collection_[middle].getX()[0]);
          if (xMiddle > x + ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon"))
            {
              upper = middle;
              xUpper = xMiddle;
            }
          else
            {
              lower = middle;
              xLower = xMiddle;
            }
        } // while
      // At this point we have upper == lower or upper == lower + 1, with lower - epsilon <= x < upper + epsilon
      // If xLower < x < xUpper, the contribution of lower must be taken into account, else it
      // must be discarded
      if (x <= xUpper - ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) return cumulativeProbabilities_[lower];
      return cumulativeProbabilities_[upper];
    }
  // Dimension > 1
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalPoint x(collection_[i].getX());
      UnsignedLong j(0);
      while ((j < dimension) && (x[j] <= point[j] + ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon"))) ++j;
      if (j == dimension) cdf += collection_[i].getP();
    }
  return cdf;
}

/* Get the PDF gradient of the distribution */
NumericalPoint UserDefined::computePDFGradient(const NumericalPoint & point) const
{
  const UnsignedLong size(collection_.getSize());
  NumericalPoint pdfGradient(size, 0.0);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      if ((point - collection_[i].getX()).norm() < ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon"))
        {
          pdfGradient[i] = 1.0;
          return pdfGradient;
        }
    }
  return pdfGradient;
}


/* Get the CDF gradient of the distribution */
NumericalPoint UserDefined::computeCDFGradient(const NumericalPoint & point) const
{
  const UnsignedLong size(collection_.getSize());
  const UnsignedLong dimension(getDimension());
  NumericalPoint cdfGradient(size, 0.0);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalPoint x(collection_[i].getX());
      UnsignedLong j(0);
      while ((j < dimension) && (point[j] <= x[j])) ++j;
      if (j == dimension) cdfGradient[i] = 1.0;
    }
  return cdfGradient;
}

/* Compute the numerical range of the distribution given the parameters values */
void UserDefined::computeRange()
{
  const UnsignedLong size(collection_.getSize());
  const UnsignedLong dimension(getDimension());
  // Return an empty interval for the empty collection case
  if (size == 0)
    {
      setRange(Interval(NumericalPoint(dimension, 1.0), NumericalPoint(dimension, 0.0)));
      return;
    }
  // The number of points is finite, so are the bounds
  const Interval::BoolCollection finiteLowerBound(dimension, true);
  const Interval::BoolCollection finiteUpperBound(dimension, true);
  NumericalPoint lowerBound(collection_[0].getX());
  NumericalPoint upperBound(lowerBound);
  for (UnsignedLong i = 1; i < size; ++i)
    {
      const NumericalPoint pt(collection_[i].getX());
      for (UnsignedLong j = 0; j < dimension; ++j)
        {
          const NumericalScalar x(pt[j]);
          if (x < lowerBound[j]) lowerBound[j] = x;
          if (x > upperBound[j]) upperBound[j] = x;
        }
    }
  setRange(Interval(lowerBound, upperBound, finiteLowerBound, finiteUpperBound));
}

/* Get the support of a discrete distribution that intersect a given interval */
NumericalSample UserDefined::getSupport(const Interval & interval) const
{
  if (interval.getDimension() != getDimension()) throw InvalidArgumentException(HERE) << "Error: the given interval has a dimension that does not match the distribution dimension.";
  NumericalSample result(0, getDimension());
  const UnsignedLong size(collection_.getSize());
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalPoint x(collection_[i].getX());
      if (interval.contains(x)) result.add(x);
    }
  return result.sort(0);
}

/* Tell if the distribution is integer valued */
Bool UserDefined::isIntegral() const
{
  if (getDimension() != 1) return false;
  const UnsignedLong size(collection_.getSize());
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalScalar x(collection_[i].getX()[0]);
      if (fabs(x - round(x)) >= ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) return false;
    }
  return true;
}

/* Compute the mean of the distribution */
void UserDefined::computeMean() const
{
  const UnsignedLong size(collection_.getSize());
  NumericalPoint mean(getDimension());
  for (UnsignedLong i = 0; i < size; ++i) mean += collection_[i].getP() * collection_[i].getX();
  mean_ = mean;
  isAlreadyComputedMean_ = true;
}

/* Compute the covariance of the distribution */
void UserDefined::computeCovariance() const
{
  const UnsignedLong size(collection_.getSize());
  const UnsignedLong dimension(getDimension());
  covariance_ = CovarianceMatrix(dimension);
  for (UnsignedLong i = 0; i < dimension; ++i) covariance_(i, i) = 0.0;
  const NumericalPoint mean(getMean());
  for (UnsignedLong k = 0; k < size; ++k)
    {
      const NumericalPoint xK(collection_[k].getX() - mean);
      const NumericalScalar pK(collection_[k].getP());
      for (UnsignedLong i = 0; i < dimension; ++i)
        for (UnsignedLong j = 0; j <= i; ++j)
          covariance_(i, j) += pK * xK[i] * xK[j];
    }
  isAlreadyComputedCovariance_ = true;
}

/* Parameters value and description accessor */
UserDefined::NumericalPointWithDescriptionCollection UserDefined::getParametersCollection() const
{
  const UnsignedLong dimension(getDimension());
  NumericalPointWithDescriptionCollection parameters(dimension + 1);
  const UnsignedLong size(collection_.getSize());
  // Loop over the dimension to extract the marginal coordinates of the support
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      NumericalPointWithDescription point(size);
      Description description(size);
      for (UnsignedLong j = 0; j < size; ++j)
        {
          point[j] = collection_[j].getX()[i];
          OSS oss;
          oss << "X^" << i << "_" << j;
          description[j] = oss;
        }
      point.setDescription(description);
      parameters[i] = point;
    }
  // Loop over the size to extract the probabilities, seen as the dependence parameters
  NumericalPointWithDescription point(size);
  Description description(size);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      point[i] = collection_[i].getP();
      OSS oss;
      oss << "p_" << i;
      description[i] = oss;
    }
  point.setDescription(description);
  point.setName(getDescription()[0]);
  parameters[dimension] = point;
  return parameters;
}

/* Get the i-th marginal distribution */
UserDefined::Implementation UserDefined::getMarginal(const UnsignedLong i) const
{
  const UnsignedLong dimension(getDimension());
  if (i >= dimension) throw InvalidArgumentException(HERE) << "The index of a marginal distribution must be in the range [0, dim-1]";
  // Special case for dimension 1
  if (dimension == 1) return clone();
  // General case
  const UnsignedLong size(collection_.getSize());
  UserDefinedPairCollection coll(size);
  for (UnsignedLong j = 0; j < size; ++j)
    {
      coll[j].setX(NumericalPoint(1, collection_[j].getX()[i]));
      coll[j].setP(collection_[j].getP());
    }
  return new UserDefined(coll);
}

/* Get the distribution of the marginal distribution corresponding to indices dimensions */
UserDefined::Implementation UserDefined::getMarginal(const Indices & indices) const
{
  const UnsignedLong dimension(getDimension());
  if (!indices.check(dimension - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal distribution must be in the range [0, dim-1] and  must be different";
  // Special case for dimension 1
  if (dimension == 1) return clone();
  // General case
  const UnsignedLong outputDimension(indices.getSize());
  const UnsignedLong size(collection_.getSize());
  UserDefinedPairCollection coll(size);
  for (UnsignedLong j = 0; j < size; ++j)
    {
      NumericalPoint x(outputDimension);
      NumericalPoint xIni(collection_[j].getX());
      for (UnsignedLong k = 0; k < outputDimension; ++k) x[k] = xIni[indices[k]];
      coll[j].setX(x);
      coll[j].setP(collection_[j].getP());
    }
  return new UserDefined(coll);
} // getMarginal(Indices)

/* Interface specific to UserDefined */

/* Pair collection accessor */
void UserDefined::setPairCollection(const UserDefinedPairCollection & collection)
{
  const UnsignedLong size(collection.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: the collection is empty";
  hasUniformWeights_ = true;
  const UnsignedLong dimension(collection[0].getX().getDimension());
  if (dimension == 0) throw InvalidArgumentException(HERE) << "Error: the points in the collection must have a dimension > 0";
  // Check if all the given probabilities are >= 0
  // Check if all the points have the same dimension
  NumericalSample firstComponent(size, 1);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      if (collection[i].getX().getDimension() != dimension) throw InvalidArgumentException(HERE) << "UserDefined distribution must have all its point with the same dimension, which is not the case here collection=" << collection;
      firstComponent[i][0] = collection[i].getX()[0];
    }
  setDimension(dimension);
  // First, sort the collection such that the sample made with the first component is in ascending order
  const NumericalSample index(firstComponent.rank(0));
  UserDefinedPairCollection sortedCollection(collection);
  for (UnsignedLong i = 0; i < size; ++i) sortedCollection[(UnsignedLong)(round(index[i][0]))] = collection[i];
  NumericalScalar sum(0.0);
  const NumericalScalar firstProbability(sortedCollection[0].getP());
  cumulativeProbabilities_ = NumericalScalarCollection(size);
  const NumericalScalar pdfEpsilon(ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultPDFEpsilon"));
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalScalar p(sortedCollection[i].getP());
      if (p < 0.0) throw InvalidArgumentException(HERE) << "UserDefined distribution must have positive probabilities, which is not the case here collection=" << collection;
      sum += p;
      cumulativeProbabilities_[i] = sum;
      hasUniformWeights_ = hasUniformWeights_ && (fabs(p - firstProbability) < pdfEpsilon);
    }
  if (sum < ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultCDFEpsilon")) throw InvalidArgumentException(HERE) << "Error: the sum of probabilities is zero.";
  // Normalize the probabilities
  for (UnsignedLong i = 0; i < size; ++i)
    {
      sortedCollection[i].setP(sortedCollection[i].getP() / sum);
      cumulativeProbabilities_[i] /= sum;
    }
  // We augment slightly the last cumulative probability, which should be equal to 1.0 but we enforce a value > 1.0. It stabilizes the sampling procedures without affecting their correctness (i.e. the algoritms are exact, not approximative)
  cumulativeProbabilities_[size - 1] = 1.0 + 2.0 * ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon");
  collection_ = sortedCollection;
  isAlreadyComputedMean_ = false;
  isAlreadyComputedCovariance_ = false;
  isAlreadyCreatedGeneratingFunction_ = false;
  computeRange();
}

UserDefined::UserDefinedPairCollection UserDefined::getPairCollection() const
{
  return collection_;
}

/* Quantile computation for dimension=1 */
NumericalScalar UserDefined::computeScalarQuantile(const NumericalScalar prob,
                                                   const Bool tail,
                                                   const NumericalScalar precision) const
{
  UnsignedLong index(0);
  const NumericalScalar p(tail ? 1 - prob : prob);
  while (cumulativeProbabilities_[index] < p) ++index;
  return collection_[index].getX()[0];
}

/* Merge the identical points of the support */
void UserDefined::compactSupport(const NumericalScalar epsilon)
{
  const UnsignedLong size(collection_.getSize());
  if (size == 0) return;
  NumericalPoint lastLocation(collection_[0].getX());
  NumericalScalar lastWeight(collection_[0].getP());
  UserDefinedPairCollection compactCollection(0);
  for (UnsignedLong i = 1; i < size; ++i)
    {
      const NumericalPoint currentLocation(collection_[i].getX());
      const NumericalScalar currentWeight(collection_[i].getP());
      // The current point must be merged
      if ((currentLocation - lastLocation).norm() <= epsilon) lastWeight += collection_[i].getP();
      else
        {
          compactCollection.add(UserDefinedPair(lastLocation, lastWeight));
          lastLocation = currentLocation;
          lastWeight = currentWeight;
        }
    }
  // Check if the loop has been ended in the middle of an aggregation of points
  // It is the case if the current weight is greater than the last weight
  if (lastWeight > collection_[size - 1].getP())
    compactCollection.add(UserDefinedPair(lastLocation, lastWeight));
  setPairCollection(compactCollection);
}

/* Method save() stores the object through the StorageManager */
void UserDefined::save(Advocate & adv) const
{
  DiscreteDistribution::save(adv);
  adv.saveAttribute( "collection_", collection_ );
  adv.saveAttribute( "cumulativeProbabilities_", cumulativeProbabilities_ );
}

/* Method load() reloads the object from the StorageManager */
void UserDefined::load(Advocate & adv)
{
  DiscreteDistribution::load(adv);
  adv.loadAttribute( "collection_", collection_ );
  adv.loadAttribute( "cumulativeProbabilities_", cumulativeProbabilities_ );
  computeRange();
}

END_NAMESPACE_OPENTURNS

