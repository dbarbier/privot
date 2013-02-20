//                                               -*- C++ -*-
/**
 *  @file  LARFSZero.cxx
 *  @brief A factory for building BasisSequence objects
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

#include "BasisSequenceFactoryImplementation.hxx"
#include "LARFSZero.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS




/* Default constructor */
LARFSZero::LARFSZero(const Bool verbose)
  : BasisSequenceFactoryImplementation(verbose)
{
  // Nothing to do
}

/* Virtual constructor */
LARFSZero * LARFSZero::clone() const
{
  return new LARFSZero( *this );
}

/* Method to create new BasisSequence objects */
BasisSequence LARFSZero::build(const NumericalSample & x,
                               const NumericalSample & y,
                               const Basis & psi) const
{
  throw NotYetImplementedException(HERE);
  return BasisSequence( psi );
}

/* String converter */
String LARFSZero::__repr__() const
{
  return "class=LARFSZero";
}

String LARFSZero::__str__(const String & offset) const
{
  return OSS(false) << offset << __repr__();
}

/* Method save() stores the object through the StorageManager */
void LARFSZero::save(Advocate & adv) const
{
  BasisSequenceFactoryImplementation::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void LARFSZero::load(Advocate & adv)
{
  BasisSequenceFactoryImplementation::load(adv);
}


END_NAMESPACE_OPENTURNS
