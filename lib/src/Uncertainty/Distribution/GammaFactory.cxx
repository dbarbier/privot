//                                               -*- C++ -*-
/**
 *  @file  GammaFactory.cxx
 *  @brief Factory for Gamma distribution
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
#include "GammaFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(GammaFactory);

/* Default constructor */
GammaFactory::GammaFactory()
  : DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
GammaFactory * GammaFactory::clone() const
{
  return new GammaFactory(*this);
}


/* Here is the interface that all derived class must implement */

GammaFactory::Implementation GammaFactory::build(const NumericalSample & sample) const
{
  return buildAsGamma(sample).clone();
}

GammaFactory::Implementation GammaFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsGamma(parameters).clone();
}

GammaFactory::Implementation GammaFactory::build() const
{
  return buildAsGamma().clone();
}

Gamma GammaFactory::buildAsGamma(const NumericalSample & sample) const
{
  UnsignedLong size(sample.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a Gamma distribution from an empty sample";
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can build a Gamma distribution only from a sample of dimension 1, here dimension=" << sample.getDimension();
  const NumericalScalar xMin(sample.getMin()[0]);
  const NumericalScalar gamma(xMin - fabs(xMin) / (2.0 + size));
  const NumericalScalar mu(sample.computeMean()[0]);
  const NumericalScalar sigma(sample.computeStandardDeviationPerComponent()[0]);
  if (sigma <= 0.0) throw InvalidArgumentException(HERE) << "Error: can build a Gamma distribution only if sigma >= 0, here sigma=" << sigma;
  NumericalScalar lambda((mu - gamma) / sigma);
  const NumericalScalar k(lambda * lambda);
  lambda /= sigma;
  return Gamma(k, lambda, gamma);
}

Gamma GammaFactory::buildAsGamma(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsGamma(RemoveDescriptionFromCollection(parameters));
}

Gamma GammaFactory::buildAsGamma(const NumericalPointCollection & parameters) const
{
  try
    {
      Gamma distribution;
      distribution.setParametersCollection(parameters);
      return distribution;
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot build a Gamma distribution from the given parameters";
    }
}

Gamma GammaFactory::buildAsGamma() const
{
  return Gamma();
}

END_NAMESPACE_OPENTURNS
