//                                               -*- C++ -*-
/**
 *  @file  NoNumericalMathGradientImplementation.hxx
 *  @brief Abstract top-level class for all numerical math gradient implementations
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
 *  Id      NoNumericalMathGradientImplementation.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_NONUMERICALMATHGRADIENTIMPLEMENTATION_HXX
#define OPENTURNS_NONUMERICALMATHGRADIENTIMPLEMENTATION_HXX

#include "NumericalMathGradientImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class NoNumericalMathGradientImplementation
 *
 * This class offers an interface for an real numerical mathematical gradient
 * implementations into the platform. It realizes the computation of the image vector
 * of a vector (aka NumericalPoint) through the gradient. The
 * gradient may be loaded in an external way through a wrapper.
 */
class NoNumericalMathGradientImplementation
  : public NumericalMathGradientImplementation
{
  CLASSNAME;
public:


  /** Default constructor */
  NoNumericalMathGradientImplementation();

  /** Virtual constructor */
  virtual NoNumericalMathGradientImplementation * clone() const;

  /** Comparison operator */
  Bool operator ==(const NoNumericalMathGradientImplementation & other) const;

  /** String converter */
  virtual String __repr__() const;



  /* Here is the interface that all derived class must implement */

  /** Gradient method */
  virtual Matrix gradient(const NumericalPoint & inP) const;

  /** Accessor for input point dimension */
  virtual UnsignedLong getInputDimension() const;

  /** Accessor for output point dimension */
  virtual UnsignedLong getOutputDimension() const;

  /** Test for actual implementation */
  virtual Bool isActualImplementation() const;

  /** Method save() stores the object through the StorageManager
   *  The inherited method is sufficient as we do not have any
   *  attribute
   */

  /** Method load() reloads the object from the StorageManager
   *  The inherited method is sufficient as we do not have any
   *  attribute
   */

protected:

  /** The gradient that actually realizes assignment between objects */

private:

}; /* class NoNumericalMathGradientImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_NONUMERICALMATHGRADIENTIMPLEMENTATION_HXX */
