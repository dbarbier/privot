//                                               -*- C++ -*-
/**
 *  @file  Sampler.cxx
 *  @brief Abstract top-level class for all Sampler
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */
#include "Sampler.hxx"
#include "RandomWalkMetropolisHastings.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(Sampler);

/* Default constructor */
Sampler::Sampler()
  : TypedInterfaceObject<SamplerImplementation>(new RandomWalkMetropolisHastings)
{
  // Nothing to do
}


/* Constructor from implementation */
Sampler::Sampler(const SamplerImplementation & op)
  : TypedInterfaceObject<SamplerImplementation>(op.clone())
{
  // Nothing to do
}


UnsignedLong Sampler::getDimension() const
{
  return getImplementation()->getDimension();
}


/* Return true if comparison succeeds */
NumericalPoint Sampler::getRealization() const
{
  return getImplementation()->getRealization();
}


/* Returns several realizations */
NumericalSample Sampler::getSample(const UnsignedLong size) const
{
  return getImplementation()->getSample(size);
}


void Sampler::setVerbose(const Bool verbose)
{
  getImplementation()->setVerbose(verbose);
}


Bool Sampler::getVerbose() const
{
  return getImplementation()->getVerbose();
}


END_NAMESPACE_OPENTURNS
