//                                               -*- C++ -*-
/**
 *  @file  Lasso.cxx
 *  @brief L1-regularized least-squares regression
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

#include "Lasso.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(Lasso);


/* Default constructor */
Lasso::Lasso(const NumericalSample & x,
             const NumericalSample & y,
             const Basis & psi)
  : ApproximationAlgorithmImplementation( x, y, psi )
{
  // Nothing to do
}

/* Default constructor */
Lasso::Lasso(const NumericalSample & x,
             const NumericalSample & y,
             const NumericalPoint & weight,
             const Basis & psi)
  : ApproximationAlgorithmImplementation( x, y, weight, psi )
{
  // Nothing to do
}

/* Virtual constructor */
Lasso * Lasso::clone() const
{
  return new Lasso( *this );
}



/* String converter */
String Lasso::__repr__() const
{
  return OSS() << "class=" << GetClassName();
}

/* Perform approximation */
void Lasso::run()
{
  throw NotYetImplementedException(HERE);
}

/* Method save() stores the object through the StorageManager */
void Lasso::save(Advocate & adv) const
{
  ApproximationAlgorithmImplementation::save(adv);
}


/* Method load() reloads the object from the StorageManager */
void Lasso::load(Advocate & adv)
{
  ApproximationAlgorithmImplementation::load(adv);
}



END_NAMESPACE_OPENTURNS
