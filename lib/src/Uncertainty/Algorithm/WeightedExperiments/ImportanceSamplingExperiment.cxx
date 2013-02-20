//                                               -*- C++ -*-
/**
 *  @file  ImportanceSamplingExperiment.cxx
 *  @brief Abstract top-level view of an monteCarloExperiment plane
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
 *  @date   2008-06-26 13:50:17 +0200 (jeu, 26 jun 2008)
 */
#include "ImportanceSamplingExperiment.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(ImportanceSamplingExperiment);


/* Default constructor */
ImportanceSamplingExperiment::ImportanceSamplingExperiment(const String & name):
  WeightedExperiment(name)
{
  // Take the default distribution as the importance distribution
  importanceDistribution_ = distribution_;
}

/* Constructor with parameters */
ImportanceSamplingExperiment::ImportanceSamplingExperiment(const Distribution & importanceDistribution,
                                                           const UnsignedLong size,
                                                           const String & name):
  WeightedExperiment(size, name),
  importanceDistribution_(importanceDistribution)
{
  // Take the default distribution as the importance distribution
}

/* Constructor with parameters */
ImportanceSamplingExperiment::ImportanceSamplingExperiment(const Distribution & distribution,
                                                           const Distribution & importanceDistribution,
                                                           const UnsignedLong size,
                                                           const String & name):
  WeightedExperiment(distribution, size, name),
  importanceDistribution_(importanceDistribution)
{
  // Check if the distributions have compatible dimensions
  if (distribution.getDimension() != importanceDistribution.getDimension()) throw InvalidArgumentException(HERE) << "Error: the distribution and the importance distribution must have the same dimension.";
}

/* Virtual constructor */
ImportanceSamplingExperiment * ImportanceSamplingExperiment::clone() const
{
  return new ImportanceSamplingExperiment(*this);
}

/* Importance distribution accessor */
Distribution ImportanceSamplingExperiment::getImportanceDistribution() const
{
  return importanceDistribution_;
}

/* String converter */
String ImportanceSamplingExperiment::__repr__() const
{
  OSS oss;
  oss << "class=" << GetClassName()
      << " name=" << getName()
      << " distribution=" << distribution_
      << " importance distribution=" << importanceDistribution_
      << " size=" << size_;
  return oss;
}

/* Sample generation with weights */
NumericalSample ImportanceSamplingExperiment::generate(NumericalPoint & weights)
{
  NumericalSample result(size_, distribution_.getDimension());
  weights_ = NumericalPoint(size_);
  for (UnsignedLong i = 0; i < size_; ++i)
    {
      result[i] = importanceDistribution_.getRealization();
      weights_[i] = distribution_.computePDF(result[i]) / importanceDistribution_.computePDF(result[i]);
    }
  weights = weights_;
  return result;
}

/* Sample generation */
NumericalSample ImportanceSamplingExperiment::generate()
{
  NumericalPoint tmp(0);
  return generate(tmp);
}

END_NAMESPACE_OPENTURNS
