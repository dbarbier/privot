//                                               -*- C++ -*-
/**
 *  @file  DualLinearCombinationHessianImplementation.hxx
 *  @brief The hessian part of functional linear combination of vectors
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
#ifndef OPENTURNS_DUALLINEARCOMBINATIONHESSIANIMPLEMENTATION_HXX
#define OPENTURNS_DUALLINEARCOMBINATIONHESSIANIMPLEMENTATION_HXX

#include "NumericalMathHessianImplementation.hxx"
#include "DualLinearCombinationEvaluationImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class DualLinearCombinationHessianImplementation
 *
 * The hessian part of functional linear combination of vectors
 */

class DualLinearCombinationHessianImplementation
  : public NumericalMathHessianImplementation
{
  CLASSNAME;
public:


  // friend class Factory<DualLinearCombinationHessianImplementation>;

  /** Default constructor */
  DualLinearCombinationHessianImplementation();

  /** Parameters constructor */
  DualLinearCombinationHessianImplementation(const DualLinearCombinationEvaluationImplementation & evaluation);

  /** Virtual constructor */
  DualLinearCombinationHessianImplementation * clone() const;

  /** Hessian method */
  SymmetricTensor hessian(const NumericalPoint & inP) const;

  /** Accessor for input point dimension */
  UnsignedLong getInputDimension() const;

  /** Accessor for output point dimension */
  UnsignedLong getOutputDimension() const;

  /** String converter */
  String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);


protected:

private:
  /** The associated evaluation */
  DualLinearCombinationEvaluationImplementation evaluation_;

} ; /* class DualLinearCombinationHessianImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_DUALLINEARCOMBINATIONHESSIANIMPLEMENTATION_HXX */
