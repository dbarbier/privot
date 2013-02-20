//                                               -*- C++ -*-
/**
 *  @file  FixedExperiment.cxx
 *  @brief Abstract top-level view of an fixedExperiment plane
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
#include "FixedExperiment.hxx"
#include "UserDefined.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(FixedExperiment);


/* Default constructor */
FixedExperiment::FixedExperiment(const String & name):
  WeightedExperiment(name),
  sample_(0, 0)
{
  // Nothing to do
}

/* Constructor with parameters */
FixedExperiment::FixedExperiment(const NumericalSample & sample,
                                 const String & name):
  WeightedExperiment(UserDefined(sample), sample.getSize(), name),
  sample_(sample)
{
  // Nothing to do
}

/* Constructor with parameters */
FixedExperiment::FixedExperiment(const NumericalSample & sample,
                                 const NumericalPoint & weights,
                                 const String & name):
  WeightedExperiment(UserDefined(sample, weights), sample.getSize(), name),
  sample_(sample)
{
  // Add the weights to the upper level class
  weights_ = weights;
}

/* Virtual constructor */
FixedExperiment * FixedExperiment::clone() const
{
  return new FixedExperiment(*this);
}

/* String converter */
String FixedExperiment::__repr__() const
{
  OSS oss;
  oss << "class=" << GetClassName()
      << " name=" << getName()
      << " sample=" << sample_
      << " weights=" << weights_;
  return oss;
}

/* Distribution accessor */
void FixedExperiment::setDistribution(const Distribution & distribution)
{
  // The distribution of a FixedExperiment cannot be changed
}

/* Sample generation */
NumericalSample FixedExperiment::generate()
{
  return sample_;
}

END_NAMESPACE_OPENTURNS
