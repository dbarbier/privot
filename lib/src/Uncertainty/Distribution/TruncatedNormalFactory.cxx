//                                               -*- C++ -*-
/**
 *  @file  TruncatedNormalFactory.cxx
 *  @brief Factory for TruncatedNormal distribution
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
#include "TruncatedNormalFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(TruncatedNormalFactory);

/* Default constructor */
TruncatedNormalFactory::TruncatedNormalFactory()
  : DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
TruncatedNormalFactory * TruncatedNormalFactory::clone() const
{
  return new TruncatedNormalFactory(*this);
}


/* Here is the interface that all derived class must implement */

TruncatedNormalFactory::Implementation TruncatedNormalFactory::build(const NumericalSample & sample) const
{
  return buildAsTruncatedNormal(sample).clone();
}

TruncatedNormalFactory::Implementation TruncatedNormalFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsTruncatedNormal(parameters).clone();
}

TruncatedNormalFactory::Implementation TruncatedNormalFactory::build() const
{
  return buildAsTruncatedNormal().clone();
}

TruncatedNormal TruncatedNormalFactory::buildAsTruncatedNormal(const NumericalSample & sample) const
{
  if (sample.getSize() == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a TruncatedNormal distribution from an empty sample";
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can build a TruncatedNormal distribution only from a sample of dimension 1, here dimension=" << sample.getDimension();
  // The method runRFactory checks the ability for R to compute parameters estimates
  const NumericalPoint parametersValue(runRFactory(sample, TruncatedNormal()));
  // Next, we try to build a TruncatedNormal distribution, which checks the parameters validity
  try
    {
      return TruncatedNormal(parametersValue[0], parametersValue[1], parametersValue[2], parametersValue[3]);
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: the estimated parameters values are not valid, mu=" << parametersValue[0] << ", sigma=" << parametersValue[1] << ", a=" << parametersValue[2] << ", b=" << parametersValue[3];
    }
}

TruncatedNormal TruncatedNormalFactory::buildAsTruncatedNormal(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsTruncatedNormal(RemoveDescriptionFromCollection(parameters));
}

TruncatedNormal TruncatedNormalFactory::buildAsTruncatedNormal(const NumericalPointCollection & parameters) const
{
  try
    {
      TruncatedNormal distribution;
      distribution.setParametersCollection(parameters);
      return distribution;
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot build a TruncatedNormal distribution from the given parameters";
    }
}

TruncatedNormal TruncatedNormalFactory::buildAsTruncatedNormal() const
{
  return TruncatedNormal();
}

END_NAMESPACE_OPENTURNS
