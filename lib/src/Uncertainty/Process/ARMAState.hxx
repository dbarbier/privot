//                                               -*- C++ -*-
/**
 *  @file  ARMAState.hxx
 *  @brief ARMAState class enables to set a State before a simulation
 * of an ARMA process and / or t get the State
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
 *  Id      ARMAState.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_ARMASTATE_HXX
#define OPENTURNS_ARMASTATE_HXX


#include "PersistentObject.hxx"
#include "NumericalSample.hxx"
#include "NumericalPoint.hxx"
#include "TimeSeries.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class ARMAState
 *
 * The implementation of ARMA State
 */
class ARMAState
  : public PersistentObject
{
  CLASSNAME;

public:

  /** Some typedefs to ease reading */

  /** Default constructors */
  ARMAState();
  ARMAState(const NumericalSample & x,
            const NumericalSample & epsilon);

  /** Virtual constructor */
  virtual ARMAState * clone() const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** Data X accessor */
  NumericalSample getX() const ;
  void setXEpsilon(const NumericalSample & x,
                   const NumericalSample & epsilon);

  /** Noise \epsilon accessor */
  NumericalSample getEpsilon() const ;

  /** Dimension accessor */
  UnsignedLong getDimension() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:

  /** Store the p last values of an ARMA(p, q) */
  NumericalSample x_;
  /** Store the q last value of the noise of an ARMA(p, q) */
  NumericalSample epsilon_;

}; /* class ARMAState */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ARMASTATE_HXX */
