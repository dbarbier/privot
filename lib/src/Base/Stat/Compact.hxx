//                                               -*- C++ -*-
/**
 *  @file  Compact.hxx
 *  @brief This class is a top-level class for the history mechanism of
 *  simulation algorithms. It delegates to its children the effective
 *  history strategy.
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
 *  @author: schueller
 *  @date:   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 *  Id:      Compact.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_COMPACT_HXX
#define OPENTURNS_COMPACT_HXX

#include "HistoryStrategyImplementation.hxx"
#include "NumericalSample.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class Compact
 */

class Compact
  : public HistoryStrategyImplementation
{

  CLASSNAME;

public:


  /** Constructor with parameters */
  explicit Compact();

  /** Constructor with parameters */
  Compact(const UnsignedLong halfMaximumSize);

  /** Virtual constructor */
  virtual Compact * clone() const;

  /** Store the point according to the strategy */
  using HistoryStrategyImplementation::store;
  void store(const NumericalPoint & point);

  /** Sample accessor */
  NumericalSample getSample() const;

  /** HalfMaximumSize accessor */
  UnsignedLong getHalfMaximumSize() const;

  /** Index accessor */
  UnsignedLong getIndex() const;

  /** String converter */
  String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);
protected:

private:
  // Maximum number of points to be hitorized
  UnsignedLong halfMaximumSize_;

  // Index where the next point will be stored
  UnsignedLong index_;;

  // Storage step
  UnsignedLong step_;

  // Throwing counter
  UnsignedLong throwingCounter_;

} ; /* class Compact */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_COMPACT_HXX */
