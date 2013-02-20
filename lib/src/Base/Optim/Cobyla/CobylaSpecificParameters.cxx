//                                               -*- C++ -*-
/**
 *  @file  CobylaSpecificParameters.cxx
 *  @brief CobylaSpecificParameters is an encapsulation of the Cobyla specific
 *         parameters.
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

#include "Cobyla.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(CobylaSpecificParameters);


/* Default with parameters */
CobylaSpecificParameters::CobylaSpecificParameters():
  PersistentObject(),
  rhoBeg_(ResourceMap::GetAsNumericalScalar( "Cobyla-DefaultRhoBeg" ))
{
  // Nothing to do
} /* SpecificParametersCobyla */

/* Parameters with parameters */
CobylaSpecificParameters::CobylaSpecificParameters(const NumericalScalar rhoBeg):
  PersistentObject()
{
  // Nothing to do
} /* SpecificParametersCobyla */

/* Virtual constructor */
CobylaSpecificParameters * CobylaSpecificParameters::clone() const
{
  return new CobylaSpecificParameters(*this);
}

/* RhoBeg accessor */
NumericalScalar CobylaSpecificParameters::getRhoBeg() const
{
  return rhoBeg_;
}

/* RhoBeg accessor */
void CobylaSpecificParameters::setRhoBeg(const NumericalScalar rhoBeg)
{
  rhoBeg_ = rhoBeg;
}

/* String converter */
String CobylaSpecificParameters::__repr__() const
{
  OSS oss;
  oss << "class=" << CobylaSpecificParameters::GetClassName()
      << " rhoBeg=" << rhoBeg_;
  return oss;
}

END_NAMESPACE_OPENTURNS



