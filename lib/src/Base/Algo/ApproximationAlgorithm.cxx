//                                               -*- C++ -*-
/**
 *  @file  ApproximationAlgorithm.cxx
 *  @brief Regression from a data sample upon a particular basis
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

#include "ApproximationAlgorithm.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(ApproximationAlgorithm);


/* Constructor from implementation */
ApproximationAlgorithm::ApproximationAlgorithm(const ApproximationAlgorithmImplementation & implementation)
  : TypedInterfaceObject<ApproximationAlgorithmImplementation>( implementation.clone() )
{
  // Nothing to do
}

/* Constructor from implementation */
ApproximationAlgorithm::ApproximationAlgorithm(const Implementation & p_implementation)
  : TypedInterfaceObject<ApproximationAlgorithmImplementation>( p_implementation )
{
  // Nothing to do
}

/* Accessors */
NumericalSample ApproximationAlgorithm::getX() const
{
  return getImplementation()->getX();
}

NumericalSample ApproximationAlgorithm::getY() const
{
  return getImplementation()->getY();
}

NumericalPoint ApproximationAlgorithm::getWeight() const
{
  return getImplementation()->getWeight();
}

Basis ApproximationAlgorithm::getPsi() const
{
  return getImplementation()->getPsi();
}

/* Verbosity accessor */
void ApproximationAlgorithm::setVerbose(const Bool verbose)
{
  copyOnWrite();
  getImplementation()->setVerbose(verbose);
}

Bool ApproximationAlgorithm::getVerbose() const
{
  return getImplementation()->getVerbose();
}

/* String converter */
String ApproximationAlgorithm::__repr__() const
{
  return getImplementation()->__repr__();
}

String ApproximationAlgorithm::__str__(const String & offset) const
{
  return getImplementation()->__str__( offset );
}

/* Perform approximation */
void ApproximationAlgorithm::run()
{
  getImplementation()->run();
}

/* Acccessor to the coefficients of the selected metamodel on the provided basis */
NumericalPoint ApproximationAlgorithm::getCoefficients()
{
  return getImplementation()->getCoefficients();
}

NumericalScalar ApproximationAlgorithm::getResidual()
{
  return getImplementation()->getResidual();
}

/* Acccessor to the residual of the selected metamodel on the provided basis */
NumericalScalar ApproximationAlgorithm::getRelativeError()
{
  return getImplementation()->getRelativeError();
}

END_NAMESPACE_OPENTURNS
