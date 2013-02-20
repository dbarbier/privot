//                                               -*- C++ -*-
/**
 *  @file  DiracFactory.cxx
 *  @brief Factory for Dirac distribution
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
 *  @date   2010-02-04 16:44:49 +0100 (jeu. 04 févr. 2010)
 */
#include "DiracFactory.hxx"
#include "DiscreteDistribution.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(DiracFactory);

/* Default constructor */
DiracFactory::DiracFactory():
  DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
DiracFactory * DiracFactory::clone() const
{
  return new DiracFactory(*this);
}


/* Here is the interface that all derived class must implement */

DiracFactory::Implementation DiracFactory::build(const NumericalSample & sample) const
{
  return buildAsDirac(sample).clone();
}

DiracFactory::Implementation DiracFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsDirac(parameters).clone();
}

DiracFactory::Implementation DiracFactory::build() const
{
  return buildAsDirac().clone();
}

Dirac DiracFactory::buildAsDirac(const NumericalSample & sample) const
{
  const UnsignedLong size(sample.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a Dirac distribution from an empty sample";
  NumericalPoint point(sample[0]);
  // Check that all the points are equal up to the discrete distribution support precision
  for (UnsignedLong i = 1; i < size; ++i) if ((point - sample[i]).norm() > ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) throw InvalidArgumentException(HERE) << "Error: cannot build a Dirac distribution from a sample containing different points";
  return Dirac(point);
}

Dirac DiracFactory::buildAsDirac(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsDirac(RemoveDescriptionFromCollection(parameters));
}

Dirac DiracFactory::buildAsDirac(const NumericalPointCollection & parameters) const
{
  try
    {
      Dirac distribution;
      distribution.setParametersCollection(parameters);
      return distribution;
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot build a Dirac distribution from the given parameters";
    }
}

Dirac DiracFactory::buildAsDirac() const
{
  return Dirac();
}

END_NAMESPACE_OPENTURNS
