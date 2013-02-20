//                                               -*- C++ -*-
/**
 *  @file  ConstantStep.hxx
 *  @brief Implementation of ConstantStep
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
 *  @author slama
 *  @date   2008-11-19 16:10:42 +0100 (mer, 19 nov 2008)
 *  Id      ConstantStep.hxx 1023 2008-11-19 15:10:42Z slama
 */

#ifndef OPENTURNS_CONSTANTSTEP_HXX
#define OPENTURNS_CONSTANTSTEP_HXX

#include "FiniteDifferenceStepImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



class ConstantStep
  : public FiniteDifferenceStepImplementation
{
  CLASSNAME;
public:


  /** Default constructor */
  ConstantStep();

  /** Parameters constructor */
  explicit ConstantStep(const NumericalPoint & eps);

  /** String converter */
  virtual String __repr__() const;

  /* Here is the interface that all derived class must implement */

  /** Virtual constructor */
  virtual ConstantStep * clone() const;

  /** Compute step */
  NumericalPoint operator()(const NumericalPoint & inP) const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);
};

END_NAMESPACE_OPENTURNS





#endif

