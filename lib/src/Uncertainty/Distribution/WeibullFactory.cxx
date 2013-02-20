//                                               -*- C++ -*-
/**
 *  @file  WeibullFactory.cxx
 *  @brief Factory for Weibull distribution
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
#include "WeibullFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(WeibullFactory);

/* Default constructor */
WeibullFactory::WeibullFactory()
  : DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
WeibullFactory * WeibullFactory::clone() const
{
  return new WeibullFactory(*this);
}

/* Here is the interface that all derived class must implement */

WeibullFactory::Implementation WeibullFactory::build(const NumericalSample & sample) const
{
  return buildAsWeibull(sample).clone();
}

WeibullFactory::Implementation WeibullFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsWeibull(parameters).clone();
}

WeibullFactory::Implementation WeibullFactory::build() const
{
  return buildAsWeibull().clone();
}

Weibull WeibullFactory::buildAsWeibull(const NumericalSample & sample) const
{
  const NumericalScalar size(sample.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a Weibull distribution from an empty sample";
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can build a Weibull distribution only from a sample of dimension 1, here dimension=" << sample.getDimension();
  const NumericalScalar xMin(sample.getMin()[0]);
  const NumericalScalar gamma(xMin - fabs(xMin) / (2.0 + size));
  const NumericalScalar mean(sample.computeMean()[0]);
  const NumericalScalar sigma(sample.computeStandardDeviationPerComponent()[0]);
  try
    {
      return Weibull(mean, sigma, gamma, Weibull::MUSIGMA);
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot estimate parameters of a Weibull distribution from the given sample";
    }
}

Weibull WeibullFactory::buildAsWeibull(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsWeibull(RemoveDescriptionFromCollection(parameters));
}

Weibull WeibullFactory::buildAsWeibull(const NumericalPointCollection & parameters) const
{
  try
    {
      Weibull distribution;
      distribution.setParametersCollection(parameters);
      return distribution;
    }
  catch (InvalidArgumentException & ex)
    {
      throw InternalException(HERE) << "Error: cannot build a Weibull distribution from the given parameters";
    }
}

Weibull WeibullFactory::buildAsWeibull() const
{
  return Weibull();
}

END_NAMESPACE_OPENTURNS
