//                                               -*- C++ -*-
/**
 *  @file  InverseBoxCoxTransform.hxx
 *  @brief Abstract top-level class for all numerical math function implementations
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
 *  Id      InverseBoxCoxTransform.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_INVERSEBOXCOXTRANSFORM_HXX
#define OPENTURNS_INVERSEBOXCOXTRANSFORM_HXX

#include "SpatialFunction.hxx"
#include "NumericalPoint.hxx"
#include "Collection.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class InverseBoxCoxTransform
 *
 * The class that simulates the classical Box Cox method which acts on the spatial part of a time series.
 */

class BoxCoxTransform;
class InverseBoxCoxTransform
  : public SpatialFunction

{
  CLASSNAME;
public:


  /** Default constructor */
  InverseBoxCoxTransform();

  /** Standard parameter constructor */
  InverseBoxCoxTransform(const NumericalPoint & lambdaPoint);

  /** NumericalScalarCollection parameter constructor */
  InverseBoxCoxTransform(const Collection<NumericalScalar> & lambdaCollection);

  /** 1D NumericalScalar parameter constructor */
  InverseBoxCoxTransform(const NumericalScalar & lambdaScalar);

  /** Virtual constructor */
  InverseBoxCoxTransform * clone() const;

  /** Inverse accessor */
  BoxCoxTransform getInverse() const;

  /** Comparison operator */
  Bool operator ==(const InverseBoxCoxTransform & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** Operator () */
  TimeSeries operator() (const TimeSeries & inTS) const;

  /** Lambda accessor */
  NumericalPoint getLambda() const;

  /** Evaluation accessor */
  EvaluationImplementation getEvaluation() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:

  /** Internal setDefaultDescription method */
  void setDefaultDescription();

  /** NumericalPoint lambda ==> get a copy is better than a dynamic_cast */
  NumericalPoint lambda_;

}; /* class InverseBoxCoxTransform */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_INVERSEBOXCOXTRANSFORM_HXX */
