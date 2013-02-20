//                                               -*- C++ -*-
/**
 *  @file  UsualDistribution.cxx
 *  @brief Abstract top-level class for all standard distributions
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
#include "UsualDistribution.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(UsualDistribution);

/* Default constructor */
UsualDistribution::UsualDistribution(const String & name)
  : DistributionImplementation(name)
{
  // Nothing to do
}

/* Virtual constructor */
UsualDistribution * UsualDistribution::clone() const
{
  return new UsualDistribution(*this);
}

/* String converter */
String UsualDistribution::__repr__() const
{
  OSS oss;
  oss << "class=" << UsualDistribution::GetClassName();
  return oss;
}

/* Tell if the distribution is elliptical */
Bool UsualDistribution::isElliptical() const
{
  return false;
}

/* Tell if the distribution is continuous */
Bool UsualDistribution::isContinuous() const
{
  return false;
}

END_NAMESPACE_OPENTURNS
