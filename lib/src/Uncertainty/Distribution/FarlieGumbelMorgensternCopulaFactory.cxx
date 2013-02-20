//                                               -*- C++ -*-
/**
 *  @file  FarlieGumbelMorgensternCopulaFactory.cxx
 *  @brief Factory for FarlieGumbelMorgensternCopula distribution
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
#include <cstdlib>
#include <cmath>

#include "FarlieGumbelMorgensternCopulaFactory.hxx"
#include "SpecFunc.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(FarlieGumbelMorgensternCopulaFactory);

/* Default constructor */
FarlieGumbelMorgensternCopulaFactory::FarlieGumbelMorgensternCopulaFactory():
  DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
FarlieGumbelMorgensternCopulaFactory * FarlieGumbelMorgensternCopulaFactory::clone() const
{
  return new FarlieGumbelMorgensternCopulaFactory(*this);
}

/* Here is the interface that all derived class must implement */

FarlieGumbelMorgensternCopulaFactory::Implementation FarlieGumbelMorgensternCopulaFactory::build(const NumericalSample & sample) const
{
  return buildAsFarlieGumbelMorgensternCopula(sample).clone();
}

FarlieGumbelMorgensternCopulaFactory::Implementation FarlieGumbelMorgensternCopulaFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsFarlieGumbelMorgensternCopula(parameters).clone();
}

FarlieGumbelMorgensternCopulaFactory::Implementation FarlieGumbelMorgensternCopulaFactory::build() const
{
  return buildAsFarlieGumbelMorgensternCopula().clone();
}

FarlieGumbelMorgensternCopula FarlieGumbelMorgensternCopulaFactory::buildAsFarlieGumbelMorgensternCopula(const NumericalSample & sample) const
{
  if (sample.getSize() == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a FarlieGumbelMorgensternCopula distribution from an empty sample";
  if (sample.getDimension() != 2) throw InvalidArgumentException(HERE) << "Error: cannot build a FarlieGumbelMorgensternCopula distribution from a sample of dimension not equal to 2";
  NumericalScalar theta(4.5 * sample.computeKendallTau().operator()(0, 1));
  // If it is not possible to use Kendall's tau to build the copula, try Spearman's rho
  if (fabs(theta) > 1.0)
    {
      LOGWARN(OSS() << "Warning! Unable to build a FarlieGumbelMorgensternCopula based on Kendall's tau: it leads to theta=" << theta << ". Trying to use Spearman's rho instead.");
      theta = 3.0 * sample.computeSpearmanCorrelation().operator()(0, 1);
      if (fabs(theta) > 1.0) throw InvalidArgumentException(HERE) << "Error: cannot build a FarlieGumbelMorgensternCopula from Spearman's rho either: it leads to theta=" << theta;
    }
  return FarlieGumbelMorgensternCopula(theta);
}

FarlieGumbelMorgensternCopula FarlieGumbelMorgensternCopulaFactory::buildAsFarlieGumbelMorgensternCopula(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsFarlieGumbelMorgensternCopula(RemoveDescriptionFromCollection(parameters));
}

FarlieGumbelMorgensternCopula FarlieGumbelMorgensternCopulaFactory::buildAsFarlieGumbelMorgensternCopula(const NumericalPointCollection & parameters) const
{
  try
    {
      FarlieGumbelMorgensternCopula copula;
      copula.setParametersCollection(parameters);
      return copula;
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot build a FarlieGumbelMorgensternCopula from the given parameters";
    }
}

FarlieGumbelMorgensternCopula FarlieGumbelMorgensternCopulaFactory::buildAsFarlieGumbelMorgensternCopula() const
{
  return FarlieGumbelMorgensternCopula();
}

END_NAMESPACE_OPENTURNS
