//                                               -*- C++ -*-
/**
 *  @file  MinCopula.cxx
 *  @brief A class that implements an independent copula
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
 *  @date   2011-11-21 23:16:45 +0100 (lun. 21 nov. 2011)
 */
#include <cmath>
#include "MinCopula.hxx"
#include "Indices.hxx"
#include "RandomGenerator.hxx"
#include "Exception.hxx"
#include "Description.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(MinCopula);

static Factory<MinCopula> RegisteredFactory("MinCopula");

/* Default constructor */
MinCopula::MinCopula(const UnsignedLong dim)
  : CopulaImplementation("MinCopula")
{
  // The range is generic for all the copulas
  setDimension(dim);
}

/* Comparison operator */
Bool MinCopula::operator ==(const MinCopula & other) const
{
  if (this == &other) return true;
  return getDimension() == other.getDimension();
}

/* String converter */
String MinCopula::__repr__() const
{
  OSS oss;
  oss << "class=" << MinCopula::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension();
  return oss;
}

String MinCopula::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(dim = " << getDimension() << ")";
  return oss;
}

/* Virtual constructor */
MinCopula * MinCopula::clone() const
{
  return new MinCopula(*this);
}

/* Get one realization of the distribution */
NumericalPoint MinCopula::getRealization() const
{
  return NumericalPoint(getDimension(), RandomGenerator::Generate());
}

/* Get the DDF of the distribution */
NumericalPoint MinCopula::computeDDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point has a dimension not compatible with the distribution dimension";
  return NumericalPoint(dimension, 0.0);
}

/* Get the PDF of the distribution */
NumericalScalar MinCopula::computePDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point has a dimension not compatible with the distribution dimension";
  const NumericalScalar u(point[0]);
  if ((u <= 0.0) || (u > 1.0)) return 0.0;
  for (UnsignedLong i = 1; i < dimension; ++i) if (u != point[i]) return 0.0;
  return u;
}

/* Get the CDF of the distribution */
NumericalScalar MinCopula::computeCDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point has a dimension not compatible with the distribution dimension";
  NumericalScalar u(point[0]);
  for (UnsignedLong i = 1; i < dimension; ++i) if (point[i] < u) u = point[i];
  return std::max(0.0, std::min(1.0, u));
} // computeCDF

/* Get the quantile of the distribution */
NumericalPoint MinCopula::computeQuantile(const NumericalScalar prob,
                                          const Bool tail) const
{
  if ((prob < 0.0) || (prob > 1.0)) throw InvalidArgumentException(HERE) << "Error: cannot compute a quantile for a probability level outside of [0, 1]";
  // Special case for boarding values
  if (prob == 0.0) return getRange().getLowerBound();
  if (prob == 1.0) return getRange().getUpperBound();
  return NumericalPoint(getDimension(), prob);
}

/* Compute the covariance of the distribution */
void MinCopula::computeCovariance() const
{
  const UnsignedLong dimension(getDimension());
  covariance_ = CovarianceMatrix(dimension);
  for (UnsignedLong i = 0; i < dimension; ++i)
    for (UnsignedLong j = 0; j <= i; ++j)
      covariance_(i, j) = 1.0 / 12.0;
  isAlreadyComputedCovariance_ = true;
}

/* Get the distribution of the marginal distribution corresponding to indices dimensions */
MinCopula::Implementation MinCopula::getMarginal(const Indices & indices) const
{
  const UnsignedLong dimension(getDimension());
  if (!indices.check(dimension - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal distribution must be in the range [0, dim-1] and  must be different";
  // Special case for dimension 1
  if (dimension == 1) return clone();
  // General case
  return new MinCopula(indices.getSize());
}

/* Get the inverse isoprobabilist transformation */
MinCopula::InverseIsoProbabilisticTransformation MinCopula::getIsoProbabilisticTransformation() const
{
  throw NotDefinedException(HERE) << "Error: no iso-probabilistic transformation can be defined for the Min copula.";
}

/* Get the inverse isoprobabilist transformation */
MinCopula::InverseIsoProbabilisticTransformation MinCopula::getInverseIsoProbabilisticTransformation() const
{
  throw NotDefinedException(HERE) << "Error: no inverse iso-probabilistic transformation can be defined for the Min copula.";
}

/* Check if the distribution is elliptical */
Bool MinCopula::isElliptical() const
{
  return false;
}

/* Check if the distribution is continuos */
Bool MinCopula::isContinuous() const
{
  return false;
}

/* Tell if the distribution has elliptical copula */
Bool MinCopula::hasEllipticalCopula() const
{
  return false;
}

/* Tell if the distribution has independent copula */
Bool MinCopula::hasIndependentCopula() const
{
  return (getDimension() == 1);
}

/* Parameters value and description accessor */
MinCopula::NumericalPointWithDescriptionCollection MinCopula::getParametersCollection() const
{
  // No parameter, return a collection with an empty NumericalPointWithDescription
  return NumericalPointWithDescriptionCollection(1, NumericalPointWithDescription(0));
} // getParametersCollection

void MinCopula::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  // Check if the given parameters are ok
  if (parametersCollection.getSize() != 1) throw InvalidArgumentException(HERE) << "Error: the given collection has a size=" << parametersCollection.getSize() << " but should be of size=1";
  if (parametersCollection[0].getDimension() != 0) throw InvalidArgumentException(HERE) << "Error: got " << parametersCollection[0].getDimension() << " instead of 0.";
}

/* Method save() stores the object through the StorageManager */
void MinCopula::save(Advocate & adv) const
{
  CopulaImplementation::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void MinCopula::load(Advocate & adv)
{
  // The range is generic for all the copulas
  CopulaImplementation::load(adv);
}

END_NAMESPACE_OPENTURNS
