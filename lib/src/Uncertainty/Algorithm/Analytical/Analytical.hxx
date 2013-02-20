//                                               -*- C++ -*-
/**
 *  @file  Analytical.hxx
 *  @brief Analytical implements an algorithm to find the design point
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 *  Id      Analytical.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_ANALYTICAL_HXX
#define OPENTURNS_ANALYTICAL_HXX

#include "Threadable.hxx"
#include "PersistentObject.hxx"
#include "NumericalPoint.hxx"
#include "NumericalPointWithDescription.hxx"
#include "NearestPointAlgorithm.hxx"
#include "NearestPointChecker.hxx"
#include "Collection.hxx"
#include "Event.hxx"
#include "StandardEvent.hxx"
#include "NumericalSample.hxx"
#include "NumericalMathFunction.hxx"
#include "Graph.hxx"
#include "Description.hxx"
#include "AnalyticalResult.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class Analytical
 * NearestPointAlgorithm implements an algorithm to find the design point
 * and computes the results of an analytical result
 */

class Analytical
  : public PersistentObject,
    public Threadable
{

  CLASSNAME;
public:

  typedef Collection<NumericalPointWithDescription> Sensitivity;

  /** Constructor with parameters */
  explicit Analytical(const NearestPointAlgorithm & nearestPointAlgorithm,
                      const Event & compositeEvent,
                      const NumericalPoint & physicalStartingPoint);


  /** Virtual constructor */
  virtual Analytical * clone() const;

  /** Physical starting point accessor */
  NumericalPoint getPhysicalStartingPoint() const;

  /** Physical starting point accessor */
  void setPhysicalStartingPoint(const NumericalPoint & physicalStartingPoint);

  /** Event accessor */
  Event getEvent() const;

  /** Event accessor */
  void setEvent(const Event & event);

  /** NearestPointAlgorithm accessor */
  NearestPointAlgorithm getNearestPointAlgorithm() const;

  /** NearestPointAlgorithm accessor */
  void setNearestPointAlgorithm(const NearestPointAlgorithm & nearestPointAlgorithm);

  /** Result accessor */
  virtual AnalyticalResult getAnalyticalResult() const;

  /** String converter */
  String __repr__() const;

  /** Performs the actual computation. */
  void run();

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

  /** For save/load mechanism*/
  Analytical() {};

  friend class Factory<Analytical>;

private:

  NearestPointAlgorithm nearestPointAlgorithm_;
  Event event_;
  NumericalPoint physicalStartingPoint_;
  AnalyticalResult result_;

} ; /* class Analytical */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ANALYTICAL_HXX */
