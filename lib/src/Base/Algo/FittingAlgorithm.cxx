//                                               -*- C++ -*-
/**
 *  @file  FittingAlgorithm.cxx
 *  @brief Cross-validation algorithm interface
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

#include "FittingAlgorithm.hxx"
#include "FittingAlgorithmImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(FittingAlgorithm);

/* Default constructor */
FittingAlgorithm::FittingAlgorithm()
  : TypedInterfaceObject<FittingAlgorithmImplementation>()
{
  // Nothing to do
}

/* Constructor from implementation */
FittingAlgorithm::FittingAlgorithm(const Implementation & p_implementation)
  : TypedInterfaceObject<FittingAlgorithmImplementation>( p_implementation )
{
  // Nothing to do
}

/* Constructor from implementation */
FittingAlgorithm::FittingAlgorithm(const FittingAlgorithmImplementation & implementation)
  : TypedInterfaceObject<FittingAlgorithmImplementation>( implementation.clone() )
{
  // Nothing to do
}

/* String converter */
String FittingAlgorithm::__repr__() const
{
  return getImplementation()->__repr__();
}

String FittingAlgorithm::__str__(const String & offset) const
{
  return getImplementation()->__str__( offset );
}

/* Perform cross-validation */
NumericalScalar FittingAlgorithm::run(const NumericalSample & x,
                                      const NumericalSample & y,
                                      const Basis & basis) const
{
  return getImplementation()->run( x, y, basis );
}


END_NAMESPACE_OPENTURNS
