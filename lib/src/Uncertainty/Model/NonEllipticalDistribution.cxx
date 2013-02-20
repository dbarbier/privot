//                                               -*- C++ -*-
/**
 *  @file  NonEllipticalDistribution.cxx
 *  @brief Abstract top-level class for non elliptical distributions
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
#include "NonEllipticalDistribution.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(NonEllipticalDistribution);

/* Default constructor */
NonEllipticalDistribution::NonEllipticalDistribution(const String & name)
  : ContinuousDistribution(name)
{
  // Nothing to do
}

/* Virtual constructor */
NonEllipticalDistribution * NonEllipticalDistribution::clone() const
{
  return new NonEllipticalDistribution(*this);
}

/* String converter */
String NonEllipticalDistribution::__repr__() const
{
  OSS oss;
  oss << "class=" << NonEllipticalDistribution::GetClassName();
  return oss;
}

END_NAMESPACE_OPENTURNS
