//                                               -*- C++ -*-
/**
 *  @file  ConstantRandomVector.cxx
 *  @brief An implementation class for constant random vectors
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
#include "PersistentObjectFactory.hxx"
#include "ConstantRandomVector.hxx"
#include "UserDefined.hxx"
#include "UserDefinedPair.hxx"
#include "SquareMatrix.hxx"

BEGIN_NAMESPACE_OPENTURNS

typedef Collection<UserDefinedPair>              UserDefinedPairCollection;

CLASSNAMEINIT(ConstantRandomVector);

static Factory<ConstantRandomVector> RegisteredFactory("ConstantRandomVector");

/* Default constructor */
ConstantRandomVector::ConstantRandomVector(const NumericalPointWithDescription & point,
                                           const String & name)
  : RandomVectorImplementation(name),
    point_(point)
{
  // Get the description from the underlying point
  setDescription( point.getDescription() );
}

/* Parameter constructor */
ConstantRandomVector::ConstantRandomVector(const NumericalPoint & point,
                                           const String & name)
  : RandomVectorImplementation(name),
    point_(point)
{
  // Get the description from the underlying point
  setDescription( point.getDimension() );
}


/* Virtual constructor */
ConstantRandomVector * ConstantRandomVector::clone() const
{
  return new ConstantRandomVector(*this);
}

/* String converter */
String ConstantRandomVector::__repr__() const
{
  OSS oss;
  oss << "class=" << ConstantRandomVector::GetClassName()
      << " description=" << getDescription()
      << " point=" << point_;
  return oss;
}



/* Here is the interface that all derived class must implement */


/* Dimension accessor */
UnsignedLong ConstantRandomVector::getDimension() const
{
  return point_.getDimension();
}

/* Realization accessor */
NumericalPoint ConstantRandomVector::getRealization() const
{
  return point_;
}

/* Numerical sample accessor */
NumericalSample ConstantRandomVector::getNumericalSample(const UnsignedLong size) const
{
  return NumericalSample(size, point_);
}

/* Mean accessor */
NumericalPoint ConstantRandomVector::getMean() const
{
  return point_;
}

/* Covariance accessor */
CovarianceMatrix ConstantRandomVector::getCovariance() const
{
  const UnsignedLong dimension(getDimension());
  CovarianceMatrix covariance(dimension);
  for (UnsignedLong i = 0; i < dimension; ++i) covariance(i, i) = 0.0;
  return covariance;
}

/* Get the random vector corresponding to the i-th marginal component */
ConstantRandomVector::Implementation ConstantRandomVector::getMarginal(const UnsignedLong i) const
{
  if (i >= getDimension()) throw InvalidArgumentException(HERE) << "The index of a marginal random vector must be in the range [0, dim-1]";
  return new ConstantRandomVector(NumericalPoint(1, point_[i]));
}

/* Get the marginal random vector corresponding to indices components */
ConstantRandomVector::Implementation ConstantRandomVector::getMarginal(const Indices & indices) const
{
  if (!indices.check(getDimension() - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal random vector must be in the range [0, dim-1] and  must be different";
  const UnsignedLong marginalDimension(indices.getSize());
  NumericalPoint marginalPoint(marginalDimension);
  for (UnsignedLong i = 0; i < marginalDimension; ++i) marginalPoint[i] = point_[indices[i]];
  return new ConstantRandomVector(marginalPoint);
}

/* Distribution accessor */
Distribution ConstantRandomVector::getDistribution() const
{
  UserDefinedPairCollection collection(1);
  collection[0].setX( point_ );
  collection[0].setP( 1.0 );
  return UserDefined(collection);
}

/* Method save() stores the object through the StorageManager */
void ConstantRandomVector::save(Advocate & adv) const
{
  RandomVectorImplementation::save(adv);
  adv.saveAttribute( "point_", point_ );
}

/* Method load() reloads the object from the StorageManager */
void ConstantRandomVector::load(Advocate & adv)
{
  RandomVectorImplementation::load(adv);
  adv.loadAttribute( "point_", point_ );
}

END_NAMESPACE_OPENTURNS
