//                                               -*- C++ -*-
/**
 *  @file  ExponentialFactory.cxx
 *  @brief Factory for Exponential distribution
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
#include "ExponentialFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(ExponentialFactory);

/* Default constructor */
ExponentialFactory::ExponentialFactory()
  : DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
ExponentialFactory * ExponentialFactory::clone() const
{
  return new ExponentialFactory(*this);
}

/* Here is the interface that all derived class must implement */

ExponentialFactory::Implementation ExponentialFactory::build(const NumericalSample & sample) const
{
  return buildAsExponential(sample).clone();
}

ExponentialFactory::Implementation ExponentialFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsExponential(parameters).clone();
}

ExponentialFactory::Implementation ExponentialFactory::build() const
{
  return buildAsExponential().clone();
}

Exponential ExponentialFactory::buildAsExponential(const NumericalSample & sample) const
{
  const UnsignedLong size(sample.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a Exponential distribution from an empty sample";
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can build an Exponential distribution only from a sample of dimension 1, here dimension=" << sample.getDimension();
  const NumericalScalar xMin(sample.getMin()[0]);
  const NumericalScalar gamma(xMin - fabs(xMin) / (2.0 + size));
  const NumericalScalar mean(sample.computeMean()[0]);
  if (mean == gamma) throw InvalidArgumentException(HERE) << "Error: can build an Exponential distribution only from a sample with mean > min, here mean=" << mean << " and min=" << gamma;
  const NumericalScalar lambda(1.0 / (mean - gamma));
  return Exponential(lambda, gamma);
}

Exponential ExponentialFactory::buildAsExponential(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsExponential(RemoveDescriptionFromCollection(parameters));
}

Exponential ExponentialFactory::buildAsExponential(const NumericalPointCollection & parameters) const
{
  try
    {
      Exponential distribution;
      distribution.setParametersCollection(parameters);
      return distribution;
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot build a Exponential distribution from the given parameters";
    }
}

Exponential ExponentialFactory::buildAsExponential() const
{
  return Exponential();
}

END_NAMESPACE_OPENTURNS
