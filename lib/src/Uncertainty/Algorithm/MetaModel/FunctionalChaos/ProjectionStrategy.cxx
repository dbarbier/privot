//                                               -*- C++ -*-
/**
 *  @file  ProjectionStrategy.cxx
 *  @brief This is the interface class for projection strategies
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
 *  @date   2008-05-21 11:21:38 +0200 (Wed, 21 May 2008)
 */
#include "ProjectionStrategy.hxx"
#include "OSS.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(ProjectionStrategy);

/* Default constructor */
ProjectionStrategy::ProjectionStrategy()
  : TypedInterfaceObject<ProjectionStrategyImplementation>(new ProjectionStrategyImplementation)
{
  // Nothing to do
}


/* Parameter constructor */
ProjectionStrategy::ProjectionStrategy(const Distribution & measure)
  : TypedInterfaceObject<ProjectionStrategyImplementation>(new ProjectionStrategyImplementation(measure))
{
  // Nothing to do
}


/* Constructor from implementation */
ProjectionStrategy::ProjectionStrategy(const ProjectionStrategyImplementation & implementation)
  : TypedInterfaceObject<ProjectionStrategyImplementation>(implementation.clone())
{
  // Nothing to do
}


/* Measure accessor */
void ProjectionStrategy::setMeasure(const Distribution & measure)
{
  copyOnWrite();
  getImplementation()->setMeasure(measure);
}

Distribution ProjectionStrategy::getMeasure() const
{
  return getImplementation()->getMeasure();
}


/* Experiment accessors */
void ProjectionStrategy::setExperiment(const WeightedExperiment & weightedExperiment)
{
  copyOnWrite();
  getImplementation()->setExperiment(weightedExperiment);
}

const WeightedExperiment & ProjectionStrategy::getExperiment() const
{
  return getImplementation()->getExperiment();
}

/* Sample accessors */
NumericalSample ProjectionStrategy::getInputSample() const
{
  return getImplementation()->getInputSample();
}

NumericalSample ProjectionStrategy::getOutputSample() const
{
  return getImplementation()->getOutputSample();
}


/* Compute the components alpha_k_p_ by projecting the model on the partial L2 basis */
void ProjectionStrategy::computeCoefficients(const NumericalMathFunction & function,
                                             const NumericalMathFunctionCollection & partialBasis,
                                             const Indices & indices,
                                             const Indices & addedRanks,
                                             const Indices & conservedRanks,
                                             const Indices & removedRanks,
                                             const UnsignedLong marginalIndex)
{
  getImplementation()->computeCoefficients(function, partialBasis, indices, addedRanks, conservedRanks, removedRanks, marginalIndex);
}


/* String converter */
String ProjectionStrategy::__repr__() const
{
  return OSS() << "class=" << getClassName()
               << " implementation=" << getImplementation()->__repr__();
}


/* String converter */
String ProjectionStrategy::__str__(const String & offset) const
{
  return __repr__();
}



END_NAMESPACE_OPENTURNS
