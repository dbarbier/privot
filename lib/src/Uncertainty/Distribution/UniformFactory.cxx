//                                               -*- C++ -*-
/**
 *  @file  UniformFactory.cxx
 *  @brief Factory for Uniform distribution
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 */
#include "UniformFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(UniformFactory);

/* Default constructor */
UniformFactory::UniformFactory()
  : DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
UniformFactory * UniformFactory::clone() const
{
  return new UniformFactory(*this);
}


/* Here is the interface that all derived class must implement */

UniformFactory::Implementation UniformFactory::build(const NumericalSample & sample) const
{
  return buildAsUniform(sample).clone();
}

UniformFactory::Implementation UniformFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsUniform(parameters).clone();
}

UniformFactory::Implementation UniformFactory::build() const
{
  return buildAsUniform().clone();
}

Uniform UniformFactory::buildAsUniform(const NumericalSample & sample) const
{
  const NumericalScalar size(sample.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a Uniform distribution from an empty sample";
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can build a Uniform distribution only from a sample of dimension 1, here dimension=" << sample.getDimension();
  const NumericalScalar xMin(sample.getMin()[0]);
  const NumericalScalar a(xMin - fabs(xMin) / (2.0 + size));
  const NumericalScalar xMax(sample.getMax()[0]);
  const NumericalScalar b(xMax + fabs(xMax) / (2.0 + size));
  if (a >= b) throw InvalidArgumentException(HERE) << "Error: can build a Uniform distribution only if a < b, here a=" << a << " and b=" << b;
  return Uniform(a, b);
}

Uniform UniformFactory::buildAsUniform(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsUniform(RemoveDescriptionFromCollection(parameters));
}

Uniform UniformFactory::buildAsUniform(const NumericalPointCollection & parameters) const
{
  try
    {
      Uniform distribution;
      distribution.setParametersCollection(parameters);
      return distribution;
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot build a Uniform distribution from the given parameters";
    }
}

Uniform UniformFactory::buildAsUniform() const
{
  return Uniform();
}

END_NAMESPACE_OPENTURNS
