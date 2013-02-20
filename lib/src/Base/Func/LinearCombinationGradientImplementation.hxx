//                                               -*- C++ -*-
/**
 *  @file  LinearCombinationGradientImplementation.hxx
 *  @brief The gradient part of linear combination of polynomials
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
 *  @date   2008-05-21 17:44:02 +0200 (Wed, 21 May 2008)
 *  Id      Object.hxx 818 2008-05-21 15:44:02Z dutka
 */
#ifndef OPENTURNS_LINEARCOMBINATIONGRADIENTIMPLEMENTATION_HXX
#define OPENTURNS_LINEARCOMBINATIONGRADIENTIMPLEMENTATION_HXX

#include "NumericalMathGradientImplementation.hxx"
#include "LinearCombinationEvaluationImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class LinearCombinationGradientImplementation
 *
 * The gradient part of linear combination of polynomials
 */

class LinearCombinationGradientImplementation
  : public NumericalMathGradientImplementation
{
  CLASSNAME;
public:


  // friend class Factory<LinearCombinationGradientImplementation>;

  /** Default constructor */
  LinearCombinationGradientImplementation();

  /** Parameters constructor */
  LinearCombinationGradientImplementation(const LinearCombinationEvaluationImplementation & evaluation);

  /** Virtual constructor */
  virtual LinearCombinationGradientImplementation * clone() const;

  /** Gradient method */
  Matrix gradient(const NumericalPoint & inP) const;

  /** Accessor for input point dimension */
  UnsignedLong getInputDimension() const;

  /** Accessor for output point dimension */
  UnsignedLong getOutputDimension() const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);


protected:

private:
  /** The associated evaluation */
  LinearCombinationEvaluationImplementation evaluation_;

} ; /* class LinearCombinationGradientImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LINEARCOMBINATIONGRADIENTIMPLEMENTATION_HXX */
