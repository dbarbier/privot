//                                               -*- C++ -*-
/**
 *  @file  PenalizedLeastSquaresAlgorithmFactory.cxx
 *  @brief A factory for building PenalizedLeastSquaresAlgorithm objects
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

#include "PersistentObjectFactory.hxx"
#include "PenalizedLeastSquaresAlgorithmFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(PenalizedLeastSquaresAlgorithmFactory);

static Factory<PenalizedLeastSquaresAlgorithmFactory> RegisteredFactory("PenalizedLeastSquaresAlgorithmFactory");

/* Default constructor */
PenalizedLeastSquaresAlgorithmFactory::PenalizedLeastSquaresAlgorithmFactory()
  : ApproximationAlgorithmImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
PenalizedLeastSquaresAlgorithmFactory * PenalizedLeastSquaresAlgorithmFactory::clone() const

{
  return new PenalizedLeastSquaresAlgorithmFactory(*this);
}

/* String converter */
String PenalizedLeastSquaresAlgorithmFactory::__repr__() const
{
  return OSS() << "class=" << GetClassName();
}

/* Method to create new PenalizedLeastSquaresAlgorithm objects */
PenalizedLeastSquaresAlgorithm * PenalizedLeastSquaresAlgorithmFactory::build(const NumericalSample & x,
                                                                              const NumericalSample & y,
                                                                              const NumericalPoint & weight,
                                                                              const Basis & psi) const
{
  return new PenalizedLeastSquaresAlgorithm( x, y, weight, psi );
}

/* Method save() stores the object through the StorageManager */
void PenalizedLeastSquaresAlgorithmFactory::save(Advocate & adv) const
{
  ApproximationAlgorithmImplementationFactory::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void PenalizedLeastSquaresAlgorithmFactory::load(Advocate & adv)
{
  ApproximationAlgorithmImplementationFactory::load(adv);
}

END_NAMESPACE_OPENTURNS
