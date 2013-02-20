//                                               -*- C++ -*-
/**
 *  @file  InverseNormalFactory.cxx
 *  @brief Factory for InverseNormal distribution
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
 *  @date   2011-04-11 12:32:27 +0200 (lun. 11 avril 2011)
 */
#include <cmath>
#include "InverseNormalFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(InverseNormalFactory);

/* Default constructor */
InverseNormalFactory::InverseNormalFactory()
  : DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
InverseNormalFactory * InverseNormalFactory::clone() const
{
  return new InverseNormalFactory(*this);
}

/* Here is the interface that all derived class must implement */

InverseNormalFactory::Implementation InverseNormalFactory::build(const NumericalSample & sample) const
{
  return buildAsInverseNormal(sample).clone();
}

InverseNormalFactory::Implementation InverseNormalFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsInverseNormal(parameters).clone();
}

InverseNormalFactory::Implementation InverseNormalFactory::build() const
{
  return buildAsInverseNormal().clone();
}

InverseNormal InverseNormalFactory::buildAsInverseNormal(const NumericalSample & sample) const
{
  const UnsignedLong size(sample.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build an InverseNormal distribution from an empty sample";
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: cannot build an InverseNormal distribution from a sample of dimension different from 1";
  const NumericalScalar mu(sample.computeMean()[0]);
  NumericalScalar sum(0.0);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalScalar xI(sample[i][0]);
      if (xI <= 0.0) throw InvalidArgumentException(HERE) << "Error: cannot build an InverseNormal distribution from a sample with non positive values";
      sum += 1.0 / sample[i][0];
    }
  const NumericalScalar lambda(1.0 / (sum / size - 1.0 / mu));
  return InverseNormal(lambda, mu);
}

InverseNormal InverseNormalFactory::buildAsInverseNormal(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsInverseNormal(RemoveDescriptionFromCollection(parameters));
}

InverseNormal InverseNormalFactory::buildAsInverseNormal(const NumericalPointCollection & parameters) const
{
  try
    {
      InverseNormal distribution;
      distribution.setParametersCollection(parameters);
      return distribution;
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot build a InverseNormal distribution from the given parameters";
    }
}

InverseNormal InverseNormalFactory::buildAsInverseNormal() const
{
  return InverseNormal();
}

END_NAMESPACE_OPENTURNS
