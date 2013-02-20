//                                               -*- C++ -*-
/**
 *  @file  StudentFactory.cxx
 *  @brief Factory for Student distribution
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
#include "StudentFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(StudentFactory);

/* Default constructor */
StudentFactory::StudentFactory()
  : DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
StudentFactory * StudentFactory::clone() const
{
  return new StudentFactory(*this);
}

/* Here is the interface that all derived class must implement */

StudentFactory::Implementation StudentFactory::build(const NumericalSample & sample) const
{
  return buildAsStudent(sample).clone();
}

StudentFactory::Implementation StudentFactory::build() const
{
  return buildAsStudent().clone();
}

Student StudentFactory::buildAsStudent(const NumericalSample & sample) const
{
  if (sample.getSize() == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a Student distribution from an empty sample";
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can build a Student distribution only from a sample of dimension 1, here dimension=" << sample.getDimension();
  const NumericalScalar mu(sample.computeMean()[0]);
  const NumericalScalar sigma(sample.computeStandardDeviationPerComponent()[0]);
  const NumericalScalar nu(2.0 + 2.0 / (sigma * sigma - 1.0));
  if (nu <= 2.0) throw InvalidArgumentException(HERE) << "Error: can build a Student distribution only if nu > 2.0, here nu=" << nu;
  return Student(nu, mu);
}

Student StudentFactory::buildAsStudent() const
{
  return Student();
}

END_NAMESPACE_OPENTURNS
