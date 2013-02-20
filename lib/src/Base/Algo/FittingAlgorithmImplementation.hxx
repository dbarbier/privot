//                                               -*- C++ -*-
/**
 *  @file  FittingAlgorithmImplementation.hxx
 *  @brief Cross-validation algorithm implementation
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
 *  Id      FittingAlgorithmImplementation.hxx 1649 2010-11-09 12:44:00Z dutka
 */
#ifndef OPENTURNS_FITTINGALGORITHMIMPLEMENTATION_HXX
#define OPENTURNS_FITTINGALGORITHMIMPLEMENTATION_HXX

#include "PersistentObject.hxx"
#include "NumericalSample.hxx"
#include "Basis.hxx"
#include "BasisSequenceFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class FittingAlgorithmImplementation
 *
 * Cross-validation algorithm implementation
 */
class FittingAlgorithmImplementation
  : public PersistentObject
{
  CLASSNAME;

public:

  /** Default constructor */
  FittingAlgorithmImplementation();

  /** Virtual constructor */
  FittingAlgorithmImplementation * clone() const = 0;

  /** String converter */
  virtual String __repr__() const;

  /** Perform cross-validation */
  virtual NumericalScalar run(const NumericalSample & x,
                              const NumericalSample & y,
                              const Basis & basis) const = 0;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

}; /* class FittingAlgorithmImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_FITTINGALGORITHMIMPLEMENTATION_HXX */
