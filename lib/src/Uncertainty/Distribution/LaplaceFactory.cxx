//                                               -*- C++ -*-
/**
 *  @file  LaplaceFactory.cxx
 *  @brief Factory for Laplace distribution
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
#include "LaplaceFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(LaplaceFactory);

/* Default constructor */
LaplaceFactory::LaplaceFactory()
  : DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
LaplaceFactory * LaplaceFactory::clone() const
{
  return new LaplaceFactory(*this);
}

/* Here is the interface that all derived class must implement */

LaplaceFactory::Implementation LaplaceFactory::build(const NumericalSample & sample) const
{
  return buildAsLaplace(sample).clone();
}

LaplaceFactory::Implementation LaplaceFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsLaplace(parameters).clone();
}

LaplaceFactory::Implementation LaplaceFactory::build() const
{
  return buildAsLaplace().clone();
}

Laplace LaplaceFactory::buildAsLaplace(const NumericalSample & sample) const
{
  if (sample.getSize() == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a Laplace distribution from an empty sample";
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can build a Laplace distribution only from a sample of dimension 1, here dimension=" << sample.getDimension();
  const UnsignedLong size(sample.getSize());
  const NumericalScalar mu(sample.computeMedianPerComponent()[0]);
  NumericalScalar tau(0.0);
  for (UnsignedLong i = 0; i < size; ++i) tau += fabs(sample[i][0] - mu);
  if (tau == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a Laplace distribution with infinite lambda.";
  return Laplace(size / tau, mu);
}

Laplace LaplaceFactory::buildAsLaplace(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsLaplace(RemoveDescriptionFromCollection(parameters));
}

Laplace LaplaceFactory::buildAsLaplace(const NumericalPointCollection & parameters) const
{
  try
    {
      Laplace distribution;
      distribution.setParametersCollection(parameters);
      return distribution;
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot build a Laplace distribution from the given parameters";
    }
}

Laplace LaplaceFactory::buildAsLaplace() const
{
  return Laplace();
}

END_NAMESPACE_OPENTURNS
