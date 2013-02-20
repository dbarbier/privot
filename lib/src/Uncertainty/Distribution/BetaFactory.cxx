//                                               -*- C++ -*-
/**
 *  @file  BetaFactory.cxx
 *  @brief Factory for Beta distribution
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
#include "BetaFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS


CLASSNAMEINIT(BetaFactory);

/* Default constructor */
BetaFactory::BetaFactory():
  DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
BetaFactory * BetaFactory::clone() const
{
  return new BetaFactory(*this);
}


/* Here is the interface that all derived class must implement */

BetaFactory::Implementation BetaFactory::build(const NumericalSample & sample) const
{
  return buildAsBeta(sample).clone();
}

BetaFactory::Implementation BetaFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsBeta(parameters).clone();
}

BetaFactory::Implementation BetaFactory::build() const
{
  return buildAsBeta().clone();
}

Beta BetaFactory::buildAsBeta(const NumericalSample & sample) const
{
  const UnsignedLong size(sample.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a Beta distribution from an empty sample";
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can build a Beta distribution only from a sample of dimension 1, here dimension=" << sample.getDimension();
  const NumericalScalar xMin(sample.getMin()[0]);
  const NumericalScalar a(xMin - fabs(xMin) / (2.0 + size));
  const NumericalScalar xMax(sample.getMax()[0]);
  const NumericalScalar b(xMax + fabs(xMax) / (2.0 + size));
  if (a >= b) throw InvalidArgumentException(HERE) << "Error: can build a Beta distribution only if a < b, here a=" << a << " and b=" << b;
  const NumericalScalar mean(sample.computeMean()[0]);
  const NumericalScalar sigma(sample.computeStandardDeviationPerComponent()[0]);
  const NumericalScalar t((b - mean) * (mean - a) / (sigma * sigma) - 1.0);
  const NumericalScalar r(t * (mean - a) / (b - a));
  return Beta(r, t, a, b);
}

Beta BetaFactory::buildAsBeta(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsBeta(RemoveDescriptionFromCollection(parameters));
}

Beta BetaFactory::buildAsBeta(const NumericalPointCollection & parameters) const
{
  try
    {
      Beta distribution;
      distribution.setParametersCollection(parameters);
      return distribution;
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot build a Beta distribution from the given parameters";
    }
}

Beta BetaFactory::buildAsBeta() const
{
  return Beta();
}


END_NAMESPACE_OPENTURNS
