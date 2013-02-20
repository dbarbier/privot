//                                               -*- C++ -*-
/**
 * @file  NoNumericalMathEvaluationImplementation.hxx
 * @brief Class for unavailable evaluation implementations
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
 * @author schueller
 * @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */

#ifndef OPENTURNS_NONUMERICALMATHEVALUATIONIMPLEMENTATION_HXX
#define OPENTURNS_NONUMERICALMATHEVALUATIONIMPLEMENTATION_HXX

#include "NumericalMathEvaluationImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class NoNumericalMathEvaluationImplementation
 *
 * Class for unavailable evaluation implementations
 */
class NoNumericalMathEvaluationImplementation
  : public NumericalMathEvaluationImplementation
{
  CLASSNAME;
public:



  /** Default constructor */
  NoNumericalMathEvaluationImplementation();

  /** Virtual constructor */
  virtual NoNumericalMathEvaluationImplementation * clone() const;

  /** Comparison operator */
  Bool operator ==(const NoNumericalMathEvaluationImplementation & other) const;

  /** String converter */
  virtual String __repr__() const;


  /** Test for actual implementation */
  virtual Bool isActualImplementation() const;


  /* Here is the interface that all derived class must implement */

  /** Operator () */
  virtual NumericalPoint operator() (const NumericalPoint & inP) const;

  /** Accessor for input point dimension */
  virtual UnsignedLong getInputDimension() const;

  /** Accessor for output point dimension */
  virtual UnsignedLong getOutputDimension() const;

  /** Method save() stores the object through the StorageManager
   *  The inherited method is sufficient as we do not have any
   *  attribute
   */

  /** Method load() reloads the object from the StorageManager
   *  The inherited method is sufficient as we do not have any
   *  attribute
   */

protected:


private:

}; /* class NoNumericalMathEvaluationImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_NONONUMERICALMATHEVALUATIONIMPLEMENTATION_HXX */
