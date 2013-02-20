//                                               -*- C++ -*-
/**
 *  @file  NormalFactory.cxx
 *  @brief Factory for Normal distribution
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
#include "NormalFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(NormalFactory);

/* Default constructor */
NormalFactory::NormalFactory()
  : DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
NormalFactory * NormalFactory::clone() const
{
  return new NormalFactory(*this);
}

/* Here is the interface that all derived class must implement */

NormalFactory::Implementation NormalFactory::build(const NumericalSample & sample) const
{
  return buildAsNormal(sample).clone();
}

NormalFactory::Implementation NormalFactory::build() const
{
  return buildAsNormal().clone();
}

NormalFactory::Implementation NormalFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsNormal(parameters).clone();
}

Normal NormalFactory::buildAsNormal(const NumericalSample & sample) const
{
  if (sample.getSize() == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a Normal distribution from an empty sample";
  const NumericalPoint mean(sample.computeMean());
  const CovarianceMatrix covariance(sample.computeCovariance());
  return Normal(mean, covariance);
}

Normal NormalFactory::buildAsNormal(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsNormal(RemoveDescriptionFromCollection(parameters));
}

Normal NormalFactory::buildAsNormal(const NumericalPointCollection & parameters) const
{
  try
    {
      UnsignedLong dimension(parameters.getSize() == 1 ? 1 : parameters.getSize() - 1);
      Normal distribution(dimension);
      distribution.setParametersCollection(parameters);
      return distribution;
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot build a Normal distribution from the given parameters" << ex;
    }
}

Normal NormalFactory::buildAsNormal() const
{
  return Normal();
}

END_NAMESPACE_OPENTURNS
