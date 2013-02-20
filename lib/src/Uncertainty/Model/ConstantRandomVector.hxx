//                                               -*- C++ -*-
/**
 *  @file  ConstantRandomVector.hxx
 *  @brief An implementation class for constant random vectors
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 *  Id      ConstantRandomVector.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_CONSTANTRANDOMVECTOR_HXX
#define OPENTURNS_CONSTANTRANDOMVECTOR_HXX

#include "OTprivate.hxx"
#include "RandomVectorImplementation.hxx"
#include "NumericalPoint.hxx"

BEGIN_NAMESPACE_OPENTURNS





/**
 * @class ConstantRandomVector
 *
 * An implementation class for constant random vectors
 */
class ConstantRandomVector
  : public RandomVectorImplementation
{
  CLASSNAME;

public:

  /** Default constructor */
  ConstantRandomVector(const NumericalPointWithDescription & point,
                       const String & name = DefaultName);

  /** Parameter constructor */
  ConstantRandomVector(const NumericalPoint & point,
                       const String & name = DefaultName);


  /** Virtual constructor */
  ConstantRandomVector * clone() const;

  /** String converter */
  String __repr__() const;




  /* Here is the interface that all derived class must implement */

  /** Dimension accessor */
  UnsignedLong getDimension() const;

  /** Realization accessor */
  NumericalPoint getRealization() const;

  /** Numerical sample accessor */
  NumericalSample getNumericalSample(const UnsignedLong size) const;

  /** Mean accessor */
  NumericalPoint getMean() const;

  /** Covariance accessor */
  CovarianceMatrix getCovariance() const;

  /** Get the random vector corresponding to the i-th marginal component */
  Implementation getMarginal(const UnsignedLong i) const;

  /** Get the marginal random vector corresponding to indices components */
  Implementation getMarginal(const Indices & indices) const;

  /** Distribution accessor */
  Distribution getDistribution() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

  friend class Factory<ConstantRandomVector>;

  /** Default constructor */
  ConstantRandomVector(const String & name = DefaultName) : RandomVectorImplementation(name) {};

private:

  /** The point where the vector realizes itself */
  NumericalPoint point_;

}; /* class ConstantRandomVector */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_CONSTANTRANDOMVECTOR_HXX */
