//                                               -*- C++ -*-
/**
 *  @file  LogUniformFactory.cxx
 *  @brief Factory for LogUniform distribution
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
 *  @date   2011-05-24 19:30:41 +0200 (mar. 24 mai 2011)
 */
#include "LogUniformFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(LogUniformFactory);

/* Default constructor */
LogUniformFactory::LogUniformFactory()
  : DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
LogUniformFactory * LogUniformFactory::clone() const
{
  return new LogUniformFactory(*this);
}


/* Here is the interface that all derived class must implement */

LogUniformFactory::Implementation LogUniformFactory::build(const NumericalSample & sample) const
{
  return buildAsLogUniform(sample).clone();
}

LogUniformFactory::Implementation LogUniformFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsLogUniform(parameters).clone();
}

LogUniformFactory::Implementation LogUniformFactory::build() const
{
  return buildAsLogUniform().clone();
}

LogUniform LogUniformFactory::buildAsLogUniform(const NumericalSample & sample) const
{
  const NumericalScalar size(sample.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a LogUniform distribution from an empty sample";
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can build a LogUniform distribution only from a sample of dimension 1, here dimension=" << sample.getDimension();
  const NumericalScalar xMin(sample.getMin()[0]);
  const NumericalScalar a(xMin - fabs(xMin) / (2.0 + size));
  if (a <= 0.0) throw InvalidArgumentException(HERE) << "Error: cannot build a LogUniform distribution from a sample that contains non positive values.";
  const NumericalScalar aLog(log(a));
  const NumericalScalar xMax(sample.getMax()[0]);
  const NumericalScalar b(xMax + fabs(xMax) / (2.0 + size));
  const NumericalScalar bLog(log(b));
  if (aLog >= bLog) throw InvalidArgumentException(HERE) << "Error: can build a LogUniform distribution only if aLog < bLog, here aLog=" << aLog << " and bLog=" << bLog;
  return LogUniform(aLog, bLog);
}

LogUniform LogUniformFactory::buildAsLogUniform(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsLogUniform(RemoveDescriptionFromCollection(parameters));
}

LogUniform LogUniformFactory::buildAsLogUniform(const NumericalPointCollection & parameters) const
{
  try
    {
      LogUniform distribution;
      distribution.setParametersCollection(parameters);
      return distribution;
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot build a LogUniform distribution from the given parameters";
    }
}

LogUniform LogUniformFactory::buildAsLogUniform() const
{
  return LogUniform();
}

END_NAMESPACE_OPENTURNS
