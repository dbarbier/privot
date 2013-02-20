//                                               -*- C++ -*-
/**
 *  @file  LAR.hxx
 *  @brief Least Angle Regression Algorithm
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
 *  Id      LAR.hxx 1649 2010-11-09 12:44:00Z dutka
 */
#ifndef OPENTURNS_LAR_HXX
#define OPENTURNS_LAR_HXX

#include <limits>
#include "Matrix.hxx"
#include "BasisSequenceFactoryImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class LAR
 *
 * A sequence of numerical math function collection.
 */
class LAR
  : public BasisSequenceFactoryImplementation
{
  CLASSNAME;
public:


  /** Default constructor */
  LAR(const Bool verbose = false);

  /** Virtual constructor */
  virtual LAR * clone() const;

  /** Method to create new BasisSequence objects */
  virtual BasisSequence build(const NumericalSample & x,
                              const NumericalSample & y,
                              const Basis & psi) const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

private:

}; /* class LAR */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LAR_HXX */
