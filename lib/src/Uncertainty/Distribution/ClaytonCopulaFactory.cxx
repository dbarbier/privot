//                                               -*- C++ -*-
/**
 *  @file  ClaytonCopulaFactory.cxx
 *  @brief Factory for ClaytonCopula distribution
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
 *  @date   2007-05-10 16:43:31 +0200 (jeu, 10 mai 2007)
 */
#include <cmath>
#include "ClaytonCopulaFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(ClaytonCopulaFactory);

/* Default constructor */
ClaytonCopulaFactory::ClaytonCopulaFactory()
  : DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
ClaytonCopulaFactory * ClaytonCopulaFactory::clone() const
{
  return new ClaytonCopulaFactory(*this);
}

/* Here is the interface that all derived class must implement */

ClaytonCopulaFactory::Implementation ClaytonCopulaFactory::build(const NumericalSample & sample) const
{
  return buildAsClaytonCopula(sample).clone();
}

ClaytonCopulaFactory::Implementation ClaytonCopulaFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsClaytonCopula(parameters).clone();
}

ClaytonCopulaFactory::Implementation ClaytonCopulaFactory::build() const
{
  return buildAsClaytonCopula().clone();
}

ClaytonCopula ClaytonCopulaFactory::buildAsClaytonCopula(const NumericalSample & sample) const
{
  if (sample.getSize() == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a ClaytonCopula distribution from an empty sample";
  if (sample.getDimension() != 2) throw InvalidArgumentException(HERE) << "Error: cannot build a ClaytonCopula distribution from a sample of dimension not equal to 2";
  NumericalScalar tau(sample.computeKendallTau().operator()(0, 1));
  if (tau == 1) throw InvalidArgumentException(HERE) << "Error: cannot build a ClaytonCopula distribution from a sample with Kendall tau equals to 1";
  return ClaytonCopula(2.0 * tau / (1.0 - tau));
}

ClaytonCopula ClaytonCopulaFactory::buildAsClaytonCopula(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsClaytonCopula(RemoveDescriptionFromCollection(parameters));
}

ClaytonCopula ClaytonCopulaFactory::buildAsClaytonCopula(const NumericalPointCollection & parameters) const
{
  try
    {
      ClaytonCopula copula;
      copula.setParametersCollection(parameters);
      return copula;
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot build a ClaytonCopula from the given parameters";
    }
}

ClaytonCopula ClaytonCopulaFactory::buildAsClaytonCopula() const
{
  return ClaytonCopula();
}

END_NAMESPACE_OPENTURNS
