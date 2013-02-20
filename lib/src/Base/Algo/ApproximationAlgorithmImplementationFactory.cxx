//                                               -*- C++ -*-
/**
 *  @file  ApproximationAlgorithmImplementationFactory.cxx
 *  @brief A factory for building ApproximationAlgorithmImplementation objects
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

#include "ApproximationAlgorithmImplementationFactory.hxx"
#include "Exception.hxx"
#include "PenalizedLeastSquaresAlgorithm.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(ApproximationAlgorithmImplementationFactory);




/* Default constructor */
ApproximationAlgorithmImplementationFactory::ApproximationAlgorithmImplementationFactory()
  : PersistentObject()
{
  // Nothing to do
}

ApproximationAlgorithmImplementationFactory * ApproximationAlgorithmImplementationFactory::clone() const

{
  return new ApproximationAlgorithmImplementationFactory( *this );
}


ApproximationAlgorithmImplementation * ApproximationAlgorithmImplementationFactory::build(const NumericalSample & x,
                                                                                          const NumericalSample & y,
                                                                                          const Basis & psi) const
{
  const UnsignedLong size( x.getSize() );
  return build( x, y, NumericalPoint( size, 1.0 ), psi );
}

ApproximationAlgorithmImplementation * ApproximationAlgorithmImplementationFactory::build(const NumericalSample & x,
                                                                                          const NumericalSample & y,
                                                                                          const NumericalPoint & weight,
                                                                                          const Basis & psi) const
{
  throw NotYetImplementedException(HERE);
  // return new PenalizedLeastSquaresAlgorithm( x, y, weight, psi );
}

String ApproximationAlgorithmImplementationFactory::__repr__() const
{
  return OSS() << "class=" << getClassName();
}

/* Method save() stores the object through the StorageManager */
void ApproximationAlgorithmImplementationFactory::save(Advocate & adv) const
{
  PersistentObject::save(adv);
}


/* Method load() reloads the object from the StorageManager */
void ApproximationAlgorithmImplementationFactory::load(Advocate & adv)
{
  PersistentObject::load(adv);
}


END_NAMESPACE_OPENTURNS
