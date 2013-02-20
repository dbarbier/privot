//                                               -*- C++ -*-
/**
 *  @file  InverseNatafIndependentCopulaHessian.hxx
 *  @brief Class for the InverseNataf transformation evaluation for independent
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
 *  Id      InverseNatafIndependentCopulaHessian.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_INVERSENATAFINDEPENDENTCOPULAHESSIAN_HXX
#define OPENTURNS_INVERSENATAFINDEPENDENTCOPULAHESSIAN_HXX

#include "NumericalMathHessianImplementation.hxx"
#include "NumericalPoint.hxx"
#include "StorageManager.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class InverseNatafIndependentCopulaHessian
 *
 * This class offers an interface for the InverseNataf function for independent distributions
 */
class InverseNatafIndependentCopulaHessian
  : public NumericalMathHessianImplementation
{
  CLASSNAME;
public:


  /** Default constructor */
  InverseNatafIndependentCopulaHessian();

  /** Parameter constructor */
  InverseNatafIndependentCopulaHessian(const UnsignedLong dimension);


  /** Virtual constructor */
  virtual InverseNatafIndependentCopulaHessian * clone() const;

  /** Hessian */
  SymmetricTensor hessian(const NumericalPoint & inP) const;

  /** Accessor for input point dimension */
  virtual UnsignedLong getInputDimension() const;

  /** Accessor for output point dimension */
  virtual UnsignedLong getOutputDimension() const;

  /** String converter */
  virtual String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:


private:
  UnsignedLong dimension_;

}; /* InverseNatafIndependentCopulaHessian */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_INVERSENATAFINDEPENDENTCOPULAHESSIAN_HXX */
