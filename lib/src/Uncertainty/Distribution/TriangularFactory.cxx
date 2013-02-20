//                                               -*- C++ -*-
/**
 *  @file  TriangularFactory.cxx
 *  @brief Factory for Triangular distribution
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
#include "TriangularFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(TriangularFactory);

/* Default constructor */
TriangularFactory::TriangularFactory()
  : DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
TriangularFactory * TriangularFactory::clone() const
{
  return new TriangularFactory(*this);
}


/* Here is the interface that all derived class must implement */

TriangularFactory::Implementation TriangularFactory::build(const NumericalSample & sample) const
{
  return buildAsTriangular(sample).clone();
}

TriangularFactory::Implementation TriangularFactory::build() const
{
  return buildAsTriangular().clone();
}

TriangularFactory::Implementation TriangularFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsTriangular(parameters).clone();
}

Triangular TriangularFactory::buildAsTriangular(const NumericalSample & sample) const
{
  const NumericalScalar size(sample.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a Triangular distribution from an empty sample";
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can build a Triangular distribution only from a sample of dimension 1, here dimension=" << sample.getDimension();

  const NumericalScalar xMin(sample.getMin()[0]);
  const NumericalScalar a(xMin - fabs(xMin) / (size + 2));
  const NumericalScalar xMax(sample.getMax()[0]);
  const NumericalScalar b(xMax + fabs(xMax) / (size + 2));
  const NumericalScalar m(3.0 * sample.computeMean()[0] - a - b);
  return Triangular(a, m, b);
}

Triangular TriangularFactory::buildAsTriangular(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsTriangular(RemoveDescriptionFromCollection(parameters));
}

Triangular TriangularFactory::buildAsTriangular(const NumericalPointCollection & parameters) const
{
  try
    {
      Triangular distribution;
      distribution.setParametersCollection(parameters);
      return distribution;
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot build a Triangular distribution from the given parameters";
    }
}

Triangular TriangularFactory::buildAsTriangular() const
{
  return Triangular();
}

END_NAMESPACE_OPENTURNS
