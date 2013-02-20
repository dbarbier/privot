//                                               -*- C++ -*-
/**
 *  @file  LogisticFactory.cxx
 *  @brief Factory for Logistic distribution
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
#include "LogisticFactory.hxx"
#include "SpecFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(LogisticFactory);

/* Default constructor */
LogisticFactory::LogisticFactory()
  : DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
LogisticFactory * LogisticFactory::clone() const
{
  return new LogisticFactory(*this);
}


/* Here is the interface that all derived class must implement */

LogisticFactory::Implementation LogisticFactory::build(const NumericalSample & sample) const
{
  return buildAsLogistic(sample).clone();
}

LogisticFactory::Implementation LogisticFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsLogistic(parameters).clone();
}

LogisticFactory::Implementation LogisticFactory::build() const
{
  return buildAsLogistic().clone();
}

Logistic LogisticFactory::buildAsLogistic(const NumericalSample & sample) const
{
  if (sample.getSize() == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a Logistic distribution from an empty sample";
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can build a Logistic distribution only from a sample of dimension 1, here dimension=" << sample.getDimension();
  NumericalScalar alpha(sample.computeMean()[0]);
  NumericalScalar beta(sample.computeStandardDeviationPerComponent()[0] * SpecFunc::SQRT3_PI);
  if (beta <= 0.0) throw InvalidArgumentException(HERE) << "Error: can build a Logistic distribution only if beta > 0.0, here beta=" << beta;
  return Logistic(alpha, beta);
}

Logistic LogisticFactory::buildAsLogistic(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsLogistic(RemoveDescriptionFromCollection(parameters));
}

Logistic LogisticFactory::buildAsLogistic(const NumericalPointCollection & parameters) const
{
  try
    {
      Logistic distribution;
      distribution.setParametersCollection(parameters);
      return distribution;
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot build a Logistic distribution from the given parameters";
    }
}

Logistic LogisticFactory::buildAsLogistic() const
{
  return Logistic();
}


END_NAMESPACE_OPENTURNS
