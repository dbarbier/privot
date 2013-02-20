//                                               -*- C++ -*-
/**
 *  @file  BasisSequenceFactoryImplementation.hxx
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
 *  Id      BasisSequenceFactoryImplementation.hxx 1649 2010-11-09 12:44:00Z dutka
 */
#ifndef OPENTURNS_BASISSEQUENCEFACTORYIMPLEMENTATION_HXX
#define OPENTURNS_BASISSEQUENCEFACTORYIMPLEMENTATION_HXX

#include <iostream>
#include "PersistentObject.hxx"
#include "BasisSequence.hxx"
#include "NumericalSample.hxx"
#include "Graph.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class BasisSequenceFactoryImplementation
 *
 * A factory for building BasisSequence objects
 */
class BasisSequenceFactoryImplementation
  : public PersistentObject
{
  CLASSNAME;
public:

  /** Default constructor */
  BasisSequenceFactoryImplementation(const Bool verbose = false);

  /** Virtual constructor */
  virtual BasisSequenceFactoryImplementation * clone() const = 0;

  /** Verbosity accessor */
  void setVerbose(const Bool verbose);
  Bool getVerbose() const;

  /** Method to create new BasisSequence objects */
  virtual BasisSequence build(const NumericalSample & x,
                              const NumericalSample & y,
                              const Basis & psi) const;

  /** Stopping criterion on the L1-norm of the coefficients accessor */
  void setMaximumRelativeConvergence(const NumericalScalar coefficientsPaths);
  NumericalScalar getMaximumRelativeConvergence() const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:
  /** Verbosity flag */
  Bool verbose_;

  /** Stopping criterion on the L1-norm of the coefficients */
  NumericalScalar maximumRelativeConvergence_;

}; /* class BasisSequenceFactoryImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_BASISSEQUENCEFACTORYIMPLEMENTATION_HXX */
