//                                               -*- C++ -*-
/**
 *  @file  FisherSnedecorFactory.cxx
 *  @brief Factory for FisherSnedecor distribution
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
#include "FisherSnedecorFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(FisherSnedecorFactory);

/* Default constructor */
FisherSnedecorFactory::FisherSnedecorFactory():
  DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
FisherSnedecorFactory * FisherSnedecorFactory::clone() const
{
  return new FisherSnedecorFactory(*this);
}

/* Here is the interface that all derived class must implement */

FisherSnedecorFactory::Implementation FisherSnedecorFactory::build(const NumericalSample & sample) const
{
  return buildAsFisherSnedecor(sample).clone();
}

FisherSnedecorFactory::Implementation FisherSnedecorFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsFisherSnedecor(parameters).clone();
}

FisherSnedecorFactory::Implementation FisherSnedecorFactory::build() const
{
  return buildAsFisherSnedecor().clone();
}

FisherSnedecor FisherSnedecorFactory::buildAsFisherSnedecor(const NumericalSample & sample) const
{
  const UnsignedLong size(sample.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a FisherSnedecor distribution from an empty sample";
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can build an FisherSnedecor distribution only from a sample of dimension 1, here dimension=" << sample.getDimension();
  const NumericalScalar mean(sample.computeMean()[0]);
  const NumericalScalar var(sample.computeCovariance()(0, 0));
  if (mean <= 1.0) throw InvalidArgumentException(HERE) << "Error: can build a FisherSnedecor distribution only from a sample with mean > 1, here mean=" << mean;
  const NumericalScalar d1(-2.0 / (mean - 1.0 + var * (1.0 - 2.0 / mean) / mean));
  const NumericalScalar d2(2.0 * mean / (mean - 1.0));
  return FisherSnedecor(d1, d2);
}

FisherSnedecor FisherSnedecorFactory::buildAsFisherSnedecor(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsFisherSnedecor(RemoveDescriptionFromCollection(parameters));
}

FisherSnedecor FisherSnedecorFactory::buildAsFisherSnedecor(const NumericalPointCollection & parameters) const
{
  try
    {
      FisherSnedecor distribution;
      distribution.setParametersCollection(parameters);
      return distribution;
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot build a FisherSnedecor distribution from the given parameters";
    }
}

FisherSnedecor FisherSnedecorFactory::buildAsFisherSnedecor() const
{
  return FisherSnedecor();
}


END_NAMESPACE_OPENTURNS
