//                                               -*- C++ -*-
/**
 *  @file  MonteCarlo.hxx
 *  @brief MonteCarlo is an implementation of the hit or miss Monte Carlo simulation method
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
 *  Id      MonteCarlo.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_MONTECARLO_HXX
#define OPENTURNS_MONTECARLO_HXX

#include "Simulation.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class MonteCarlo
 */

class MonteCarlo :
  public Simulation
{

  CLASSNAME;
public:


  /** Constructor with parameters */
  MonteCarlo(const Event & event);

  /** for save/load mecanism */
  MonteCarlo() {};

  /** Virtual constructor */
  virtual MonteCarlo * clone() const;

  /** String converter */
  String __repr__() const;

private:

  /** Compute the block sample and the points that realized the event */
  NumericalSample computeBlockSample();

} ; /* class MonteCarlo */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_MONTECARLO_HXX */
