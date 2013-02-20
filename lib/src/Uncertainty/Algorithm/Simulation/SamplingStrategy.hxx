//                                               -*- C++ -*-
/**
 *  @file  SamplingStrategy.hxx
 *  @brief
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
 *  Id      SamplingStrategy.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_SAMPLINGSTRATEGY_HXX
#define OPENTURNS_SAMPLINGSTRATEGY_HXX

#include "TypedInterfaceObject.hxx"
#include "SamplingStrategyImplementation.hxx"
#include "TypedInterfaceObject.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class SamplingStrategy
 */

class SamplingStrategy
  : public TypedInterfaceObject<SamplingStrategyImplementation>
{

  CLASSNAME;

public:

  /** Default constructor */
  explicit SamplingStrategy();

  /** Constructor with parameters */
  SamplingStrategy(const SamplingStrategyImplementation & implementation);

  /** Constructor with parameters */
  SamplingStrategy(Implementation & p_implementation);

  /** Constructor with parameters */
  explicit SamplingStrategy(const UnsignedLong dimension);


  /** Generate a set of directions */
  NumericalSample generate() const;

  /** Dimension accessor */
  void setDimension(const UnsignedLong dimension);
  UnsignedLong getDimension() const;

  /** String converter */
  String __repr__() const;

} ; /* class SamplingStrategy */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SAMPLINGSTRATEGY_HXX */
