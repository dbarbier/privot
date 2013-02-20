//                                               -*- C++ -*-
/**
 *  @file  RandomVector.cxx
 *  @brief The class that implements all random vectors
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
#include "RandomVector.hxx"
#include "ConstantRandomVector.hxx"
#include "UsualRandomVector.hxx"
#include "CompositeRandomVector.hxx"
#include "EventRandomVectorImplementation.hxx"
#include "EventDomainImplementation.hxx"
#include "EventProcess.hxx"
#include "FunctionalChaosRandomVector.hxx"
#include "ComparisonOperatorImplementation.hxx"
#include "Domain.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(RandomVector);

/* Default constructor */
RandomVector:: RandomVector()
  : TypedInterfaceObject<RandomVectorImplementation>(new RandomVectorImplementation())
{
  // Nothing to do
}

/* Parameters constructor */
RandomVector::RandomVector(const RandomVectorImplementation & implementation,
                           const String & name)
  : TypedInterfaceObject<RandomVectorImplementation>(implementation.clone())
{
  getImplementation()->setName(name);
}

/* Constructor from implementation */
RandomVector::RandomVector(const Implementation & p_implementation,
                           const String & name)
  : TypedInterfaceObject<RandomVectorImplementation>(p_implementation)
{
  getImplementation()->setName(name);
}

/* Constructor from implementation pointer */
RandomVector::RandomVector(RandomVectorImplementation * p_implementation,
                           const String & name)
  : TypedInterfaceObject<RandomVectorImplementation>(p_implementation)
{
  getImplementation()->setName(name);
}

/* Constructor for constant vector */
RandomVector::RandomVector(const NumericalPoint & point,
                           const String & name)
  : TypedInterfaceObject<RandomVectorImplementation>(new ConstantRandomVector(point, name))
{
  // Nothing to do
}

/* Constructor for distribution-based vector */
RandomVector::RandomVector(const Distribution & distribution,
                           const String & name)
  : TypedInterfaceObject<RandomVectorImplementation>(new UsualRandomVector(distribution, name))
{
  // Nothing to do
}

/* Constructor for distribution-based conditional vector */
RandomVector::RandomVector(const Distribution & distribution,
                           const RandomVector & randomParameters,
                           const String & name)
  : TypedInterfaceObject<RandomVectorImplementation>(new UsualRandomVector(distribution, name))
{
  // Nothing to do
}

/* Constructor for composite vector */
RandomVector::RandomVector(const NumericalMathFunction & function,
                           const RandomVector & antecedent,
                           const String & name)
  : TypedInterfaceObject<RandomVectorImplementation>(new CompositeRandomVector(function,
                                                                               antecedent.getImplementation(),
                                                                               name))
{
  // Nothing to do
}

/* Constructor for functional chaos vector */
RandomVector::RandomVector(const FunctionalChaosResult & functionalChaosResult)
  : TypedInterfaceObject<RandomVectorImplementation>(new FunctionalChaosRandomVector(functionalChaosResult))
{
  // Nothing to do
}

/* Constructor from event RandomVector */
RandomVector::RandomVector(const RandomVector & antecedent,
                           const ComparisonOperator & op,
                           const NumericalScalar threshold,
                           const String & name)
  : TypedInterfaceObject<RandomVectorImplementation>(new EventRandomVectorImplementation(*antecedent.getImplementation(), op, threshold, name))
{
  // Nothing to do
}

/* Constructor from domain event */
RandomVector::RandomVector(const RandomVector & antecedent,
                           const Domain & domain,
                           const String & name)
  : TypedInterfaceObject<RandomVectorImplementation>(new EventDomainImplementation(*antecedent.getImplementation(), domain, name))
{
  // Nothing to do
}


/* Constructor from domain event */
RandomVector::RandomVector(const Process & process,
                           const Domain & domain,
                           const String & name)
  : TypedInterfaceObject<RandomVectorImplementation>(new EventProcess(process, domain, name))
{
  // Nothing to do
}


/* String converter */
String RandomVector::__repr__() const
{
  OSS oss;
  oss << "class=" << RandomVector::GetClassName()
      << " implementation=" << getImplementation()->__repr__();
  return oss;
}

/* String converter */
String RandomVector::__str__(const String & offset) const
{
  return __repr__();
}



/* Description Accessor */
void RandomVector::setDescription(const Description & description)
{
  copyOnWrite();
  getImplementation()->setDescription(description);
}


/* Description Accessor */
Description RandomVector::getDescription() const
{
  return getImplementation()->getDescription();
}


/* Is the underlying random vector composite ? */
Bool RandomVector::isComposite() const
{
  return getImplementation()->isComposite();
}


/* Here is the interface that all derived class may implement */

/* Dimension accessor */
UnsignedLong RandomVector::getDimension() const
{
  return getImplementation()->getDimension();
}

/* Realization accessor */
NumericalPoint RandomVector::getRealization() const
{
  return getImplementation()->getRealization();
}

/* Numerical sample accessor */
NumericalSample RandomVector::getSample(UnsignedLong size) const
{
  return getImplementation()->getSample(size);
}

NumericalSample RandomVector::getNumericalSample(UnsignedLong size) const
{
  return getImplementation()->getNumericalSample(size);
}

/* Mean accessor */
NumericalPoint RandomVector::getMean() const
{
  return getImplementation()->getMean();
}

/* Covariance accessor */
CovarianceMatrix RandomVector::getCovariance() const
{
  return getImplementation()->getCovariance();
}

/* Get the random vector corresponding to the i-th marginal component */
RandomVector RandomVector::getMarginal(const UnsignedLong i) const
{
  return *(getImplementation()->getMarginal(i));
}

/* Get the random vector corresponding to the i-th marginal component */
RandomVector RandomVector::getMarginal(const Indices & indices) const
{
  return *(getImplementation()->getMarginal(indices));
}

/* This method allows to access the antecedent RandomVector in case of a composite RandomVector */
RandomVector::Antecedent RandomVector::getAntecedent() const
{
  return getImplementation()->getAntecedent();
}

/* This method allows to access the NumericalMathFunction in case of a composite RandomVector */
NumericalMathFunction RandomVector::getFunction() const
{
  return getImplementation()->getFunction();
}

/* This method allows to access the Distribution in case of a usual RandomVector */
Distribution RandomVector::getDistribution() const
{
  return getImplementation()->getDistribution();
}

/* Operator accessor */
ComparisonOperator RandomVector::getOperator() const
{
  return getImplementation()->getOperator();
}

/* Threshold accessor */
NumericalScalar RandomVector::getThreshold() const
{
  return getImplementation()->getThreshold();
}

END_NAMESPACE_OPENTURNS
