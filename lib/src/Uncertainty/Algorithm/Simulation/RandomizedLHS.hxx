//                                               -*- C++ -*-
/**
 *  @file  RandomizedLHS.hxx
 *  @brief RandomizedLHS is an implementation of the randomized Latin Hypercube Sampling method
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
 *  @author dutka
 *  @date   2011-11-17 13:46:13 +0100 (jeu. 17 nov. 2011)
 *  Id      RandomizedLHS.hxx 2192 2011-11-17 12:46:13Z dutka
 */
#ifndef OPENTURNS_RANDOMIZEDLHS_HXX
#define OPENTURNS_RANDOMIZEDLHS_HXX

#include "Simulation.hxx"
#include "Collection.hxx"
#include "PersistentCollection.hxx"
#include "Distribution.hxx"
#include "Matrix.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class RandomizedLHS
 */

class RandomizedLHS: public Simulation
{

  CLASSNAME;
public:

  typedef Collection<Distribution>           Marginals;
  typedef PersistentCollection<Distribution> PersistentMarginals;

  /** Default constructor */
  RandomizedLHS();

  /** Constructor with parameters */
  RandomizedLHS(const Event & event);


  /** Virtual constructor */
  virtual RandomizedLHS * clone() const;

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

  /** Cells shuffle */
  Matrix shuffle_;

  /** Marginal distributions */
  PersistentMarginals marginals_;

private:

} ; /* class RANDOMIZEDLHS */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_RANDOMIZEDLHS_HXX */
