//                                               -*- C++ -*-
/**
 *  @file  ChiFactory.cxx
 *  @brief Factory for Chi distribution
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
 *  @date   2007-05-10 16:43:31 +0200 (jeu. 10 mai 2007)
 */
#include "ChiFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(ChiFactory);

/* Default constructor */
ChiFactory::ChiFactory()
  : DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
ChiFactory * ChiFactory::clone() const
{
  return new ChiFactory(*this);
}

/* Here is the interface that all derived class must implement */

ChiFactory::Implementation ChiFactory::build(const NumericalSample & sample) const
{
  return buildAsChi(sample).clone();
}

ChiFactory::Implementation ChiFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsChi(parameters).clone();
}

ChiFactory::Implementation ChiFactory::build() const
{
  return buildAsChi().clone();
}

Chi ChiFactory::buildAsChi(const NumericalSample & sample) const
{
  const UnsignedLong size(sample.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a Chi distribution from an empty sample";
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can build a Chi distribution only from a sample of dimension 1, here dimension=" << sample.getDimension();
  NumericalScalar sumSquares(0.0);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalScalar xI(sample[i][0]);
      sumSquares += xI * xI;
    }
  try
    {
      return Chi( sumSquares / size );
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot estimate parameters of a Chi distribution from the given sample";
    }
}

Chi ChiFactory::buildAsChi(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsChi(RemoveDescriptionFromCollection(parameters));
}

Chi ChiFactory::buildAsChi(const NumericalPointCollection & parameters) const
{
  try
    {
      Chi distribution;
      distribution.setParametersCollection(parameters);
      return distribution;
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot build a Chi distribution from the given parameters";
    }
}

Chi ChiFactory::buildAsChi() const
{
  return Chi();
}

END_NAMESPACE_OPENTURNS
