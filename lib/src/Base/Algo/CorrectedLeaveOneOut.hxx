//                                               -*- C++ -*-
/**
 *  @file  CorrectedLeaveOneOut.hxx
 *  @brief Corrected implicit leave-one-out cross validation
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
 *  Id      CorrectedLeaveOneOut.hxx 1649 2010-11-09 12:44:00Z dutka
 */
#ifndef OPENTURNS_CORRECTEDLEAVEONEOUT_HXX
#define OPENTURNS_CORRECTEDLEAVEONEOUT_HXX

#include "FittingAlgorithmImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class CorrectedLeaveOneOut
 *
 * Corrected implicit leave-one-out cross validation
 */
class CorrectedLeaveOneOut
  : public FittingAlgorithmImplementation
{
  CLASSNAME;
public:

  /** Default constructor */
  CorrectedLeaveOneOut();

  /** Virtual constructor */
  CorrectedLeaveOneOut * clone() const;

  /** String converter */
  virtual String __repr__() const;

  /** Perform cross-validation */
  virtual NumericalScalar run(const NumericalSample & x,
                              const NumericalSample & y,
                              const Basis & basis) const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

}; /* class CorrectedLeaveOneOut */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_CORRECTEDLEAVEONEOUT_HXX */
