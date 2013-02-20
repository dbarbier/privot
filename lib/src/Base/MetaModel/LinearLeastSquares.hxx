//                                               -*- C++ -*-
/**
 *  @file  LinearLeastSquares.hxx
 *  @brief First order polynomial response surface by least square
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
 *  Id      LinearLeastSquares.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_LINEARLEASTSQUARES_HXX
#define OPENTURNS_LINEARLEASTSQUARES_HXX

#include "OTprivate.hxx"
#include "PersistentObject.hxx"
#include "NumericalPoint.hxx"
#include "NumericalSample.hxx"
#include "NumericalMathFunction.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class LinearLeastSquares
 *
 * The class describes the concept of quadratic response surface
 */
class LinearLeastSquares
  : public PersistentObject
{
  CLASSNAME;
public:


  /** Constructor with parameters */
  explicit LinearLeastSquares(const NumericalSample & dataIn,
                              const NumericalMathFunction & inputFunction,
                              const String & name = DefaultName);

  /** Constructor with parameters */
  explicit LinearLeastSquares(const NumericalSample & dataIn,
                              const NumericalSample & dataOut,
                              const String & name = DefaultName);


  /** Virtual constructor */
  virtual LinearLeastSquares * clone() const;

  /** String converter */
  String __repr__() const;

  /** Response surface computation */
  void run();

  /** Constant accessor */
  NumericalPoint getConstant() const;

  /** Linear accessor */
  Matrix getLinear() const;

  /** Input function accessor */
  NumericalMathFunction getInputFunction() const;

  /** Response surface accessor */
  NumericalMathFunction getResponseSurface() const;

  /** Data in accessor */
  NumericalSample getDataIn() const;

  /** Data out accessor */
  void setDataOut(const NumericalSample & dataOut);
  NumericalSample getDataOut();

protected:

private:
  NumericalSample dataIn_;
  NumericalSample dataOut_;
  NumericalMathFunction inputFunction_;
  NumericalMathFunction responseSurface_;
  NumericalPoint constant_;
  Matrix linear_;

}; /* class LinearLeastSquares */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_QUADRATICLEASTSQUARES_HXX */
