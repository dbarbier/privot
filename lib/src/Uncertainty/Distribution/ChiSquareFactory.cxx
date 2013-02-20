//                                               -*- C++ -*-
/**
 *  @file  ChiSquareFactory.cxx
 *  @brief Factory for ChiSquare distribution
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
#include "ChiSquareFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(ChiSquareFactory);

/* Default constructor */
ChiSquareFactory::ChiSquareFactory():
  DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
ChiSquareFactory * ChiSquareFactory::clone() const
{
  return new ChiSquareFactory(*this);
}


/* Here is the interface that all derived class must implement */

ChiSquareFactory::Implementation ChiSquareFactory::build(const NumericalSample & sample) const
{
  return buildAsChiSquare(sample).clone();
}

ChiSquareFactory::Implementation ChiSquareFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsChiSquare(parameters).clone();
}

ChiSquareFactory::Implementation ChiSquareFactory::build() const
{
  return buildAsChiSquare().clone();
}

ChiSquare ChiSquareFactory::buildAsChiSquare(const NumericalSample & sample) const
{
  if (sample.getSize() == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a ChiSquare distribution from an empty sample";
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can build a ChiSquare distribution only from a sample of dimension 1, here dimension=" << sample.getDimension();
  const NumericalScalar mean(sample.computeMean()[0]);
  return ChiSquare(mean);
}

ChiSquare ChiSquareFactory::buildAsChiSquare(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsChiSquare(RemoveDescriptionFromCollection(parameters));
}

ChiSquare ChiSquareFactory::buildAsChiSquare(const NumericalPointCollection & parameters) const
{
  try
    {
      ChiSquare distribution;
      distribution.setParametersCollection(parameters);
      return distribution;
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot build a ChiSquare distribution from the given parameters";
    }
}

ChiSquare ChiSquareFactory::buildAsChiSquare() const
{
  return ChiSquare();
}


END_NAMESPACE_OPENTURNS
