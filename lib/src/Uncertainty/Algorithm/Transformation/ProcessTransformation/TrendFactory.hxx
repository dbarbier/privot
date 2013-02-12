//                                               -*- C++ -*-
/**
 *  @file  TrendFactory.hxx
 *  @brief This class is enables to estimate trend
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
 *  @author haddad
 *  @date   2011-12-19 14:30:01 +0100 (lun. 19 déc. 2011)
 *  Id      TrendFactory.hxx 2258 2011-12-19 13:30:01Z haddad
 */
#ifndef OPENTURNS_TRENDFACTORY_HXX
#define OPENTURNS_TRENDFACTORY_HXX

#include "PersistentObject.hxx"
#include "NumericalSample.hxx"
#include "TimeSeries.hxx"
#include "TrendTransform.hxx"
#include "FittingAlgorithm.hxx"
#include "BasisSequenceFactory.hxx"
#include "CorrectedLeaveOneOut.hxx"
#include "LAR.hxx"
#include "Collection.hxx"
#include "Basis.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class TrendFactory
 */

class TrendFactory
  : public PersistentObject
{

  CLASSNAME;

public:

  /** Standard constructor  */
  TrendFactory(const BasisSequenceFactory & basisSequenceFactory = LAR(),
               const FittingAlgorithm & fittingAlgorithm = CorrectedLeaveOneOut(),
               const String & name = DefaultName);

  /** BasisSequenceFactory accessors */
  BasisSequenceFactory getBasisSequenceFactory() const;
  void setBasisSequenceFactory(const BasisSequenceFactory & basisSequenceFactory);

  /** FittingAlgorithm accessors */
  FittingAlgorithm getFittingAlgorithm() const;
  void setFittingAlgorithm(const FittingAlgorithm & fittingAlgorithm);

  /** Virtual copy constructor */
  TrendFactory * clone() const;

  /** String converter */
  String __repr__() const;

  /** String converter */
  String __str__(const String & offset = "") const;

  /** Build the factory from data  */
  TrendTransform build(const TimeSeries & timeSeries,
		       const Basis & basis) const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);


private:

  /** BasisSequenceFactory argument - Default is LAR */
  BasisSequenceFactory basisSequenceFactory_;

  /** FittingAlgorithm used in evaluation */
  FittingAlgorithm fittingAlgorithm_;

} ; /* class TrendFactory */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_TRENDFACTORY_HXX */
