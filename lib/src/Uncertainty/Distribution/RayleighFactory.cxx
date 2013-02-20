//                                               -*- C++ -*-
/**
 *  @file  RayleighFactory.cxx
 *  @brief Factory for Rayleigh distribution
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
 *  @date   2008-09-24 09:07:16 +0200 (mer. 24 sept. 2008)
 */
#include <cmath>
#include "RayleighFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(RayleighFactory);

/* Default constructor */
RayleighFactory::RayleighFactory()
  : DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
RayleighFactory * RayleighFactory::clone() const
{
  return new RayleighFactory(*this);
}

/* Here is the interface that all derived class must implement */

RayleighFactory::Implementation RayleighFactory::build(const NumericalSample & sample) const
{
  return buildAsRayleigh(sample).clone();
}

RayleighFactory::Implementation RayleighFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsRayleigh(parameters).clone();
}

RayleighFactory::Implementation RayleighFactory::build() const
{
  return buildAsRayleigh().clone();
}

Rayleigh RayleighFactory::buildAsRayleigh(const NumericalSample & sample) const
{
  const UnsignedLong size(sample.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a Rayleigh distribution from an empty sample";
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can build a Rayleigh distribution only from a sample of dimension 1, here dimension=" << sample.getDimension();
  const NumericalScalar xMin(sample.getMin()[0]);
  const NumericalScalar gamma(xMin - fabs(xMin) / (2.0 + size));
  NumericalScalar sumSquares(0.0);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalScalar xI(sample[i][0] - gamma);
      sumSquares += xI * xI;
    }
  try
    {
      return Rayleigh(sqrt(0.5 * sumSquares / size), gamma);
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot estimate parameters of a Rayleigh distribution from the given sample";
    }
}

Rayleigh RayleighFactory::buildAsRayleigh(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsRayleigh(RemoveDescriptionFromCollection(parameters));
}

Rayleigh RayleighFactory::buildAsRayleigh(const NumericalPointCollection & parameters) const
{
  try
    {
      Rayleigh distribution;
      distribution.setParametersCollection(parameters);
      return distribution;
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot build a Rayleigh distribution from the given parameters";
    }
}

Rayleigh RayleighFactory::buildAsRayleigh() const
{
  return Rayleigh();
}

END_NAMESPACE_OPENTURNS
