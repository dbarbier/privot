//                                               -*- C++ -*-
/**
 *  @file  DiscreteDistribution.cxx
 *  @brief Abstract top-level class for Discrete distributions
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */
#include <cstdlib>

#include "DiscreteDistribution.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(DiscreteDistribution);

/* Default constructor */
DiscreteDistribution::DiscreteDistribution(const String & name)
  : UsualDistribution(name)
{
  // Nothing to do
}

/* Virtual constructor */
DiscreteDistribution * DiscreteDistribution::clone() const
{
  return new DiscreteDistribution(*this);
}

/* Comparison operator */
Bool DiscreteDistribution::operator ==(const DiscreteDistribution & other) const
{
  if (this == &other) return true;
  return false;
}

/* String converter */
String DiscreteDistribution::__repr__() const
{
  OSS oss;
  oss << "class=" << DiscreteDistribution::GetClassName();
  return oss;
}

/* Tell if the distribution is continuous */
Bool DiscreteDistribution::isContinuous() const
{
  return false;
}

/* Tell if the distribution is discrete */
Bool DiscreteDistribution::isDiscrete() const
{
  return true;
}

/* Tell if the distribution is integer valued */
Bool DiscreteDistribution::isIntegral() const
{
  return true;
}

END_NAMESPACE_OPENTURNS
