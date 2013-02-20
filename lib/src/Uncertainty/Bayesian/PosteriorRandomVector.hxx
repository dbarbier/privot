//                                               -*- C++ -*-
/**
 *  @file  PosteriorRandomVector.hxx
 *  @brief An implementation class for random vectors based on samplers
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
 */
#ifndef OPENTURNS_POSTERIORRANDOMVECTOR_HXX
#define OPENTURNS_POSTERIORRANDOMVECTOR_HXX

#include "RandomVectorImplementation.hxx"
#include "Sampler.hxx"

BEGIN_NAMESPACE_OPENTURNS


/**
 * @class PosteriorRandomVector
 *
 * An implementation class for functionalChaos random vectors
 */
class PosteriorRandomVector
  : public RandomVectorImplementation
{
  CLASSNAME;

public:

  /** Some typedefs to ease reading */

  /** Default constructor */
  PosteriorRandomVector(const Sampler & sampler);


  /** Virtual constructor */
  PosteriorRandomVector * clone() const;

  /** String converter */
  String __repr__() const;

  /** Sampler accessor */
  Sampler getSampler() const;

  /* Here is the interface that all derived class must implement */

  /** Dimension accessor */
  virtual UnsignedLong getDimension() const;

  /** Realization accessor */
  virtual NumericalPoint getRealization() const;

  /** Numerical sample accessor */
  virtual NumericalSample getSample(const UnsignedLong size) const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:

  friend class Factory<PosteriorRandomVector>;

  /** Default constructor */
  PosteriorRandomVector() : RandomVectorImplementation() {};

private:
  /** The underlying sampler */
  Sampler sampler_;

}; /* class PosteriorRandomVector */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_POSTERIORRANDOMVECTOR_HXX */
