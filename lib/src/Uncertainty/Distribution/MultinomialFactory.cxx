//                                               -*- C++ -*-
/**
 *  @file  MultinomialFactory.cxx
 *  @brief Factory for Multinomial distribution
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
#include <cmath>
#include "MultinomialFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(MultinomialFactory);

/* Default constructor */
MultinomialFactory::MultinomialFactory()
  : DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
MultinomialFactory * MultinomialFactory::clone() const
{
  return new MultinomialFactory(*this);
}


/* Here is the interface that all derived class must implement */

MultinomialFactory::Implementation MultinomialFactory::build(const NumericalSample & sample) const
{
  return buildAsMultinomial(sample).clone();
}

MultinomialFactory::Implementation MultinomialFactory::build() const
{
  return buildAsMultinomial().clone();
}

Multinomial MultinomialFactory::buildAsMultinomial(const NumericalSample & sample) const
{
  if (sample.getSize() == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a Multinomial distribution from an empty sample";
  // Check if each component of the sample is an integer, compute the mean and extract the maximum value
  UnsignedLong size(sample.getSize());
  UnsignedLong dimension(sample.getDimension());
  NumericalPoint p(dimension, 0.0);
  NumericalScalar max(sample[0][0]);
  for (UnsignedLong i = 0; i < size; i++)
    {
      for (UnsignedLong j = 0; j < dimension; j++)
        {
          NumericalScalar x(sample[i][j]);
          if ((x != trunc(x)) || (x < 0.0)) throw InvalidArgumentException(HERE) << "Error: can build a Multinomial distribution only from a sample with positive integer components, here sample[" << i << "][" << j << "]=" << x;
          if (x > max) max = x;
          p[j] += x;
        }
    }
  UnsignedLong n((UnsignedLong)max);
  p *= 1.0 / (max * size);
  return Multinomial(n, p);
}

Multinomial MultinomialFactory::buildAsMultinomial() const
{
  return Multinomial();
}

END_NAMESPACE_OPENTURNS
