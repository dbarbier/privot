//                                               -*- C++ -*-
/**
 *  @file  Dirac.cxx
 *  @brief The Dirac distribution
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
 *  @author dutka
 *  @date   2010-07-12 15:45:44 +0200 (lun. 12 juil. 2010)
 */
#include <cmath>
#include "Dirac.hxx"
#include "SpecFunc.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Dirac);

static Factory<Dirac> RegisteredFactory("Dirac");

/* Default constructor */
Dirac::Dirac()
  : DiscreteDistribution("Dirac"),
    point_(1, 0.0)
{
  // We set the dimension of the Dirac distribution
  setDimension( 1 );
  computeRange();
}

/* Parameters constructor */
Dirac::Dirac(const NumericalScalar p)
  : DiscreteDistribution("Dirac"),
    point_(1, p)
{
  // We set the dimension of the Dirac distribution
  setDimension( 1 );
  computeRange();
}

/* Parameters constructor */
Dirac::Dirac(const NumericalPoint & point)
  : DiscreteDistribution("Dirac"),
    point_(point)
{
  // We set the dimension of the Dirac distribution
  setDimension( point.getDimension() );
  computeRange();
}

/* Comparison operator */
Bool Dirac::operator ==(const Dirac & other) const
{
  if (this == &other) return true;
  return point_ == other.point_;
}

/* String converter */
String Dirac::__repr__() const
{
  OSS oss;
  oss << "class=" << Dirac::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " point=" << point_;
  return oss;
}

String Dirac::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(point = " << point_ << ")";
  return oss;
}

/* Virtual constructor */
Dirac * Dirac::clone() const
{
  return new Dirac(*this);
}

/* Get one realization of the distribution */
NumericalPoint Dirac::getRealization() const
{
  return point_;
}


/* Get the PDF of the distribution */
NumericalScalar Dirac::computePDF(const NumericalPoint & point) const
{
  if (point.getDimension() != getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute the PDF on a point with dimension=" << point.getDimension() << " different from the distribution dimension=" << getDimension();
  if ((point - point_).norm() <= ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) return 1.0;
  return 0.0;
}


/* Get the CDF of the distribution */
NumericalScalar Dirac::computeCDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: cannot compute the CDF on a point with dimension=" << point.getDimension() << " different from the distribution dimension=" << getDimension();
  // If at least one component is too small
  for (UnsignedLong i = 0; i < dimension; ++i) if (point_[i] > point[i]) return 0.0;
  return 1.0;
}

/* Compute the multivariate quantile of the distribution */
NumericalPoint Dirac::computeQuantile(const NumericalScalar prob,
                                      const Bool tail) const
{
  NumericalScalar quantileEpsilon(ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon"));
  if (prob < -quantileEpsilon || prob > 1.0 + quantileEpsilon) throw InvalidArgumentException(HERE) << "Error: cannot compute a quantile for a probability level outside of [0, 1]";
  return point_;
}

/* Get the PDF gradient of the distribution */
NumericalPoint Dirac::computePDFGradient(const NumericalPoint & point) const
{
  if (point.getDimension() != getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute the PDF gradient on a point with dimension=" << point.getDimension() << " different from the distribution dimension=" << getDimension();
  throw NotYetImplementedException(HERE);
}


/* Get the CDF gradient of the distribution */
NumericalPoint Dirac::computeCDFGradient(const NumericalPoint & point) const
{
  if (point.getDimension() != getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute the CDF gradient on a point with dimension=" << point.getDimension() << " different from the distribution dimension=" << getDimension();
  throw NotYetImplementedException(HERE);
}

/* Get the quantile of the distribution */
NumericalScalar Dirac::computeScalarQuantile(const NumericalScalar prob,
                                             const Bool tail,
                                             const NumericalScalar precision) const
{
  return point_[0];
}

/* Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
NumericalComplex Dirac::computeCharacteristicFunction(const NumericalScalar u) const
{
  return exp(computeLogCharacteristicFunction(u));
}

NumericalComplex Dirac::computeLogCharacteristicFunction(const NumericalScalar u) const
{
  if (getDimension() != 1) throw NotYetImplementedException(HERE) << "Error: the computeCharacteristicFunction() method is implemented for 1D distributions only.";
  const NumericalComplex value(0.0, u * point_[0]);
  return value;
}

/* Get the generating function of the distribution, i.e. psi(z) = E(z^X) */
NumericalComplex Dirac::computeGeneratingFunction(const NumericalComplex & z) const
{
  return exp(computeLogGeneratingFunction(z));
}

NumericalComplex Dirac::computeLogGeneratingFunction(const NumericalComplex & z) const
{
  if (getDimension() != 1) throw NotYetImplementedException(HERE) << "Error: the computeGeneratingFunction() method is implemented for 1D distributions only.";
  return point_[0] * log(z);
}

/* Compute the mean of the distribution */
void Dirac::computeMean() const
{
  mean_ = point_;
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint Dirac::getStandardDeviation() const
{
  return NumericalPoint(getDimension(), 0.0);
}

/* Get the skewness of the distribution */
NumericalPoint Dirac::getSkewness() const
{
  return NumericalPoint(getDimension(), 0.0);
}

/* Get the kurtosis of the distribution */
NumericalPoint Dirac::getKurtosis() const
{
  return NumericalPoint(getDimension(), 0.0);
}

/* Get the moments of the standardized distribution */
NumericalPoint Dirac::getStandardMoment(const UnsignedLong n) const
{
  const UnsignedLong dimension(getDimension());
  NumericalPoint result(dimension);
  for (UnsignedLong i = 0; i < dimension; ++i) result[i] = pow(point_[i], n);
  return result;
}

/* Compute the covariance of the distribution */
void Dirac::computeCovariance() const
{
  covariance_ = CovarianceMatrix(getDimension());
  isAlreadyComputedCovariance_ = true;
}

/* Get the support of a discrete distribution that intersect a given interval */
NumericalSample Dirac::getSupport(const Interval & interval) const
{
  if (interval.getDimension() != getDimension()) throw InvalidArgumentException(HERE) << "Error: the given interval has a dimension that does not match the distribution dimension.";
  if (interval.contains(point_)) return NumericalSample(1, point_);
  return NumericalSample(0, getDimension());
}

/* Parameters value and description accessor */
Dirac::NumericalPointWithDescriptionCollection Dirac::getParametersCollection() const
{
  const UnsignedLong dimension(getDimension());
  NumericalPointWithDescriptionCollection parameters(dimension);
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      NumericalPointWithDescription point(1);
      Description description(point.getDimension());
      point[0] = point_[i];
      description[0] = String(OSS() << "point_" << i);
      point.setDescription(description);
      point.setName(getDescription()[i]);
      parameters[i] = point;
    }
  return parameters;
}

void Dirac::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  const UnsignedLong dimension(parametersCollection.getSize());
  NumericalPoint point(dimension);
  // The ith component of the point is supposed to be the first component of the point
  // at position i in the parameters collection
  for (UnsignedLong i = 0; i < dimension; ++i)
    point[i] = parametersCollection[i][0];
  *this = Dirac(point);
}

/* Point accessor */
void Dirac::setPoint(const NumericalPoint & point)
{
  point_ = point;
  isAlreadyComputedMean_ = false;
  isAlreadyComputedCovariance_ = false;
  computeRange();
}

/* Point accessor */
NumericalPoint Dirac::getPoint() const
{
  return point_;
}


/* Compute the numerical range of the distribution given the parameters values */
void Dirac::computeRange()
{
  setRange(Interval(point_, point_));
}

/* Get the i-th marginal distribution */
Dirac::Implementation Dirac::getMarginal(const UnsignedLong i) const
{
  const UnsignedLong dimension(getDimension());
  if (i >= dimension) throw InvalidArgumentException(HERE) << "The index of a marginal distribution must be in the range [0, dim-1]";
  if (dimension == 1) return clone();
  return new Dirac(point_[i]);
}

/* Get the distribution of the marginal distribution corresponding to indices dimensions */
Dirac::Implementation Dirac::getMarginal(const Indices & indices) const
{
  const UnsignedLong dimension(getDimension());
  if (!indices.check(dimension - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal distribution must be in the range [0, dim-1] and  must be different";
  if (dimension == 1) return clone();
  const UnsignedLong outputDimension(indices.getSize());
  NumericalPoint pointMarginal(outputDimension);
  for (UnsignedLong i = 0; i < outputDimension; ++i) pointMarginal[i] = point_[indices[i]];
  return new Dirac(pointMarginal);
} // getMarginal(Indices)

/* Method save() stores the object through the StorageManager */
void Dirac::save(Advocate & adv) const
{
  DiscreteDistribution::save(adv);
  adv.saveAttribute( "point_", point_ );
}

/* Method load() reloads the object from the StorageManager */
void Dirac::load(Advocate & adv)
{
  DiscreteDistribution::load(adv);
  adv.loadAttribute( "point_", point_ );
}

END_NAMESPACE_OPENTURNS
