//                                               -*- C++ -*-
/**
 *  @file  LinearTaylor.hxx
 *  @brief First order polynomial response surface by Taylor expansion
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
 *  Id      LinearTaylor.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_LINEARTAYLOR_HXX
#define OPENTURNS_LINEARTAYLOR_HXX

#include "OTprivate.hxx"
#include "PersistentObject.hxx"
#include "NumericalPoint.hxx"
#include "Matrix.hxx"
#include "NumericalMathFunction.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class LinearTaylor
 *
 * The class describes the concept of linear taylor response surface
 */
class LinearTaylor
  : public PersistentObject
{
  CLASSNAME;
public:



  /** Default constructor */
  explicit LinearTaylor(const String & name = DefaultName);

  /** Constructor with parameters */
  explicit LinearTaylor(const NumericalPoint & center,
                        const NumericalMathFunction & inputFunction,
                        const String & name = DefaultName);


  /** Virtual constructor */
  virtual LinearTaylor * clone() const;

  /** String converter */
  String __repr__() const;

  /** Response surface computation */
  void run();

  /** Center accessor */
  NumericalPoint getCenter() const;

  /** Constant accessor */
  NumericalPoint getConstant() const;

  /** Linear accessor */
  Matrix getLinear() const;

  /** Input function accessor */
  NumericalMathFunction getInputFunction() const;

  /** Response surface accessor */
  NumericalMathFunction getResponseSurface() const;

protected:

private:

  NumericalPoint center_;
  NumericalMathFunction inputFunction_;
  NumericalMathFunction responseSurface_;
  NumericalPoint constant_;
  Matrix linear_;

}; /* class LinearTaylor */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LINEARTAYLOR_HXX */
