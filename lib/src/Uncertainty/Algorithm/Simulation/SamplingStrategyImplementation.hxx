//                                               -*- C++ -*-
/**
 *  @file  SamplingStrategyImplementation.hxx
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
 *  Id      SamplingStrategyImplementation.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_SAMPLINGSTRATEGYIMPLEMENTATION_HXX
#define OPENTURNS_SAMPLINGSTRATEGYIMPLEMENTATION_HXX

#include "PersistentObject.hxx"
#include "NumericalPoint.hxx"
#include "NumericalSample.hxx"
#include "PersistentObject.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class SamplingStrategyImplementation
 */

class SamplingStrategyImplementation
  : public PersistentObject
{

  CLASSNAME;

public:

  /** Constructor with parameters */
  explicit SamplingStrategyImplementation(const UnsignedLong dimension);

  /** Virtual constructor */
  virtual SamplingStrategyImplementation * clone() const;

  /** Generate a set of directions */
  virtual NumericalSample generate() const;

  /** Generate a uniform random unit vector */
  NumericalPoint getUniformUnitVectorRealization(const UnsignedLong dimension) const;

  /** Generate a uniform random unit vector */
  NumericalPoint getUniformUnitVectorRealization() const;

  /** Dimension accessor */
  void setDimension(const UnsignedLong dimension);
  UnsignedLong getDimension() const;

  /** String converter */
  virtual String __repr__() const;

protected:

  UnsignedLong dimension_;

} ; /* class SamplingStrategyImplementation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SAMPLINGSTRATEGYIMPLEMENTATION_HXX */
