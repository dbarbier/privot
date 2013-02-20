//                                               -*- C++ -*-
/**
 *  @file  RandomVectorImplementation.cxx
 *  @brief An interface for all implementation class of random vectors
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
#include "PersistentObjectFactory.hxx"
#include "RandomVectorImplementation.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(RandomVectorImplementation);

static Factory<RandomVectorImplementation> RegisteredFactory("RandomVectorImplementation");


/* Default constructor */
RandomVectorImplementation::RandomVectorImplementation(const String & name)
  : PersistentObject(name),
    description_()
{
  // Nothing to do
}

/* Virtual constructor */
RandomVectorImplementation * RandomVectorImplementation::clone() const
{
  return new RandomVectorImplementation(*this);
}

/* String converter */
String RandomVectorImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << RandomVectorImplementation::GetClassName();
  return oss;
}



/* Description accessor */
void RandomVectorImplementation::setDescription(const Description & description)
{
  description_ = description;
}

Description RandomVectorImplementation::getDescription() const
{
  return description_;
}


/* Here is the interface that all derived class must implement */


/* Is the underlying random vector composite ? */
Bool RandomVectorImplementation::isComposite() const
{
  return false;
}

/* Dimension accessor */
UnsignedLong RandomVectorImplementation::getDimension() const
{
  throw NotYetImplementedException(HERE);
}

/* Realization accessor */
NumericalPoint RandomVectorImplementation::getRealization() const
{
  throw NotYetImplementedException(HERE);
}

/* Numerical sample accessor */
NumericalSample RandomVectorImplementation::getSample(const UnsignedLong size) const
{
  NumericalSample result(size, getDimension());
  for (UnsignedLong i = 0; i < size; ++i) result[i] = getRealization();
  return result;
}

NumericalSample RandomVectorImplementation::getNumericalSample(const UnsignedLong size) const
{
  LOGUSER(OSS() << "In class RandomVector, method getNumericalSample is deprecated in favor of getSample.");
  return getSample(size);
}

/* Mean accessor */
NumericalPoint RandomVectorImplementation::getMean() const
{
  throw NotYetImplementedException(HERE);
}

/* Covariance accessor */
CovarianceMatrix RandomVectorImplementation::getCovariance() const
{
  throw NotYetImplementedException(HERE);
}

/* Get the random vector corresponding to the i-th marginal component */
RandomVectorImplementation::Implementation RandomVectorImplementation::getMarginal(const UnsignedLong i) const
{
  throw NotYetImplementedException(HERE);
}

/* Get the marginal random vector corresponding to indices components */
RandomVectorImplementation::Implementation RandomVectorImplementation::getMarginal(const Indices & indices) const
{
  throw NotYetImplementedException(HERE);
}

/* Fake method due to pbs with dynamic_cast and Pointer */
RandomVectorImplementation::Antecedent RandomVectorImplementation::getAntecedent() const
{
  throw NotYetImplementedException(HERE);
}

/* Fake method due to pbs with dynamic_cast and Pointer */
NumericalMathFunction RandomVectorImplementation::getFunction() const
{
  throw NotYetImplementedException(HERE);
}

/* Fake method due to pbs with dynamic_cast and Pointer */
Distribution RandomVectorImplementation::getDistribution() const
{
  throw NotYetImplementedException(HERE);
}

/* Fake method due to pbs with dynamic_cast and Pointer */
ComparisonOperator RandomVectorImplementation::getOperator() const
{
  throw NotYetImplementedException(HERE);
}

/* Fake method due to pbs with dynamic_cast and Pointer */
Process RandomVectorImplementation::getProcess() const
{
  throw NotYetImplementedException(HERE);
}

/* Fake method due to pbs with dynamic_cast and Pointer */
Domain RandomVectorImplementation::getDomain() const
{
  throw NotYetImplementedException(HERE);
}

/* Fake method due to pbs with dynamic_cast and Pointer */
NumericalScalar RandomVectorImplementation::getThreshold() const
{
  throw NotYetImplementedException(HERE);
}

/* Method save() stores the object through the StorageManager */
void RandomVectorImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "description_", description_ );
}

/* Method load() reloads the object from the StorageManager */
void RandomVectorImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "description_", description_ );
}

END_NAMESPACE_OPENTURNS
