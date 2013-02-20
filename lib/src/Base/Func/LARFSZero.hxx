//                                               -*- C++ -*-
/**
 *  @file  LARFSZero.hxx
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
 *  Id      LARFSZero.hxx 1649 2010-11-09 12:44:00Z dutka
 */
#ifndef OPENTURNS_LARFSZERO_HXX
#define OPENTURNS_LARFSZERO_HXX

#include "BasisSequenceFactoryImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class LARFSZero
 *
 * A sequence of numerical math function collection.
 */
class LARFSZero
  : public BasisSequenceFactoryImplementation
{
public:


  /** Default constructor */
  LARFSZero(const Bool verbose = false);

  /** Virtual constructor */
  virtual LARFSZero * clone() const;

  /** Method to create new BasisSequence objects */
  virtual BasisSequence build(const NumericalSample & x,
                              const NumericalSample & y,
                              const Basis & psi) const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /* Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /* Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

}; /* class LARFSZero */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LARFSZERO_HXX */
