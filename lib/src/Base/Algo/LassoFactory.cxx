//                                               -*- C++ -*-
/**
 *  @file  LassoFactory.cxx
 *  @brief A factory for building Lasso objects
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
 *  @date   2010-11-09 13:44:00 +0100 (Tue, 09 Nov 2010)
 */

#include "LassoFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



/* Default constructor */
LassoFactory::LassoFactory()
  : ApproximationAlgorithmImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
LassoFactory * LassoFactory::clone() const
{
  return new LassoFactory(*this);
}

/* String converter */
String LassoFactory::__repr__() const
{
  return OSS() << "class=" << GetClassName();
}

/* Method to create new Lasso objects */
Lasso * LassoFactory::build(const NumericalSample & x,
                            const NumericalSample & y,
                            const NumericalPoint & weight,
                            const Basis & psi) const
{
  return new Lasso( x, y, weight, psi );
}


END_NAMESPACE_OPENTURNS
