//                                               -*- C++ -*-
/**
 *  @file  LHS.hxx
 *  @brief LHS is an implementation of the hit or miss Monte Carlo simulation method
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
 *  Id      LHS.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_LHS_HXX
#define OPENTURNS_LHS_HXX

#include "Simulation.hxx"
#include "Collection.hxx"
#include "PersistentCollection.hxx"
#include "Distribution.hxx"
#include "Matrix.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class LHS
 */

class LHS: public Simulation
{

  CLASSNAME;
public:

  typedef Collection<Distribution>           Marginals;
  typedef PersistentCollection<Distribution> PersistentMarginals;

  /** Default constructor */
  LHS();

  /** Constructor with parameters */
  LHS(const Event & event);


  /** Virtual constructor */
  virtual LHS * clone() const;

  /** Performs the actual computation. */
  void run();

  /** String converter */
  String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

  /** Compute the block sample */
  NumericalSample computeBlockSample();

  /** Second antecedent dimension */
  UnsignedLong dimension_;

  /** Active block index */
  UnsignedLong blockIndex_;

  /** Cells shuffle */
  Matrix shuffle_;

  /** Marginal distributions */
  PersistentMarginals marginals_;

private:

} ; /* class LHS */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LHS_HXX */
