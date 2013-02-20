//                                               -*- C++ -*-
/**
 *  @file  ComposedNumericalMathFunction.hxx
 *  @brief The class that implements composed numerical math functions
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
 *  Id      ComposedNumericalMathFunction.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_COMPOSEDNUMERICALMATHFUNCTION_HXX
#define OPENTURNS_COMPOSEDNUMERICALMATHFUNCTION_HXX

#include "NumericalMathFunctionImplementation.hxx"
#include "NumericalMathFunction.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class ComposedNumericalMathFunction
 */

class ComposedNumericalMathFunction
  : public NumericalMathFunctionImplementation
{
  CLASSNAME;
public:

  /* Some typedefs for easy reading */

  /** Default constructor */
  ComposedNumericalMathFunction();

  /** Composition constructor */
  ComposedNumericalMathFunction(const Implementation & p_left,
                                const Implementation & p_right);

  /** Composition constructor */
  ComposedNumericalMathFunction(const NumericalMathFunction & left,
                                const NumericalMathFunction & right);

  /** Virtual constructor */
  virtual ComposedNumericalMathFunction * clone() const;

  /** Comparison operator */
  Bool operator ==(const ComposedNumericalMathFunction & other) const;

  /** String converter */
  virtual String __repr__() const;



  /** Gradient according to the marginal parameters */
  virtual Matrix parametersGradient(const NumericalPoint & inP) const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

  //ComposedNumericalMathFunction() {};
  //friend class Factory<ComposedNumericalMathFunction>;

private:

  /** The f function in fog */
  Implementation p_leftFunction_;

  /** The g function in fog */
  Implementation p_rightFunction_;

}; /* class ComposedNumericalMathFunction */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_COMPOSEDNUMERICALMATHFUNCTION_HXX */
