//                                               -*- C++ -*-
/**
 *  @file  ImportanceSampling.cxx
 *  @brief ImportanceSampling is an implementation of the importance sampling Monte Carlo simulation method
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
#include "ImportanceSampling.hxx"
#include "ComparisonOperatorImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



/*
 * @class ImportanceSampling
 */

CLASSNAMEINIT(ImportanceSampling);

/* Constructor with parameters */
ImportanceSampling::ImportanceSampling(const Event & event, const Distribution & importanceDistribution):
  Simulation(event),
  importanceDistribution_(importanceDistribution)
{
  // Check if the importance distribution dimension is compatible with the event
  if (importanceDistribution.getDimension() != event.getImplementation()->getAntecedent()->getDimension()) throw InvalidArgumentException(HERE) << "The importance distribution must have the same dimension as the event";
}

/* Virtual constructor */
ImportanceSampling * ImportanceSampling::clone() const
{
  return new ImportanceSampling(*this);
}

/* Compute the block sample */
NumericalSample ImportanceSampling::computeBlockSample()
{
  const UnsignedLong blockSize(getBlockSize());
  // First, compute a sample of the importance distribution
  const NumericalSample inputSample(importanceDistribution_.getSample(blockSize));
  // Then, evaluate the function on this sample
  NumericalSample blockSample(getEvent().getImplementation()->getFunction()(inputSample));
  // Then, modify in place this sample to take into account the change in the input distribution
  // realizedEventSample = NumericalSample(blockSize_, inputSample.getDimension());
  for (UnsignedLong i = 0; i < blockSize; i++)
    {
      const Bool isRealized(getEvent().getOperator()(blockSample[i][0], getEvent().getThreshold()));
      if (isRealized)
        {
          // If the event occured, the value is p_initial(x[i]) / p_importance(x[i])
          // Having access to p_initial is a long trip...
          const NumericalScalar weight(getEvent().getImplementation()->getAntecedent()->getDistribution().computePDF(inputSample[i]) / importanceDistribution_.computePDF(inputSample[i]));
          blockSample[i][0] = weight;
        }
      else blockSample[i][0] = 0.0;
    }
  return blockSample;
}

/* Importance distribution accessor */
Distribution ImportanceSampling::getImportanceDistribution() const
{
  return importanceDistribution_;
}

/* String converter */
String ImportanceSampling::__repr__() const
{
  OSS oss;
  oss << "class=" << ImportanceSampling::GetClassName()
      << " derived from " << Simulation::__repr__();
  return oss;
}

END_NAMESPACE_OPENTURNS
