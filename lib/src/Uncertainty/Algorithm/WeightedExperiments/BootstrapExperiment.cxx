//                                               -*- C++ -*-
/**
 *  @file  BootstrapExperiment.cxx
 *  @brief Abstract top-level view of an bootstrapExperiment plane
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
#include "BootstrapExperiment.hxx"
#include "UserDefined.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(BootstrapExperiment);


/* Default constructor */
BootstrapExperiment::BootstrapExperiment(const String & name):
  WeightedExperiment(name)
{
  // Nothing to do
}

/* Constructor with parameters */
BootstrapExperiment::BootstrapExperiment(const NumericalSample & sample,
                                         const String & name):
  WeightedExperiment(UserDefined(sample), sample.getSize(), name)
{
  // Nothing to do
}

/* Virtual constructor */
BootstrapExperiment * BootstrapExperiment::clone() const
{
  return new BootstrapExperiment(*this);
}

/* String converter */
String BootstrapExperiment::__repr__() const
{
  OSS oss;
  oss << "class=" << GetClassName()
      << " name=" << getName()
      << " distribution=" << distribution_
      << " size=" << size_;
  return oss;
}

/* Sample generation */
NumericalSample BootstrapExperiment::generate()
{
  return distribution_.getSample(size_);
}

END_NAMESPACE_OPENTURNS
