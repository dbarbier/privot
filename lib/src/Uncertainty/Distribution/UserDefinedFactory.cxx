//                                               -*- C++ -*-
/**
 *  @file  UserDefinedFactory.cxx
 *  @brief Factory for UserDefined distribution
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
#include "UserDefinedFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

typedef Collection<UserDefinedPair> UserDefinedPairCollection;

CLASSNAMEINIT(UserDefinedFactory);

/* Default constructor */
UserDefinedFactory::UserDefinedFactory()
  : DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
UserDefinedFactory * UserDefinedFactory::clone() const
{
  return new UserDefinedFactory(*this);
}


/* Here is the interface that all derived class must implement */

UserDefinedFactory::Implementation UserDefinedFactory::build(const NumericalSample & sample) const
{
  return buildAsUserDefined(sample).clone();
}

UserDefinedFactory::Implementation UserDefinedFactory::build() const
{
  return buildAsUserDefined().clone();
}

UserDefined UserDefinedFactory::buildAsUserDefined(const NumericalSample & sample) const
{
  const UnsignedLong size(sample.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a UserDefined distribution from an empty sample";
  const NumericalScalar p(1.0 / NumericalScalar(size));
  UserDefinedPairCollection support(size);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      support[i].setX(sample[i]);
      support[i].setP(p);
    }
  return UserDefined(support);
}

UserDefined UserDefinedFactory::buildAsUserDefined() const
{
  return UserDefined();
}


END_NAMESPACE_OPENTURNS
