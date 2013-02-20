//                                               -*- C++ -*-
/**
 *  @file  InverseTrendTransform.hxx
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
 *  @author lebrun
 *  @date   2011-12-19 23:37:21 +0100 (lun. 19 déc. 2011)
 *  Id      InverseTrendTransform.hxx 2269 2011-12-19 22:37:21Z lebrun
 */
#ifndef OPENTURNS_INVERSETRENDTRANSOFORM_HXX
#define OPENTURNS_INVERSETRENDTRANSOFORM_HXX


#include "TemporalFunction.hxx"
#include "NumericalMathFunction.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class InverseTrendTransform
 *
 * The class that simulates a dynamical function based on a
 * numerical math function that acts only on the temporal part
 * of a time series.
 */

class TrendTransform;

class InverseTrendTransform
  : public TemporalFunction
{
  CLASSNAME;
public:

  typedef NumericalMathEvaluationImplementation::Implementation EvaluationImplementation;

  /** Default constructor */
  InverseTrendTransform();

  /** Parameter constructor */
  InverseTrendTransform(const NumericalMathFunction & function);

  /** Parameter constructor */
  InverseTrendTransform(const EvaluationImplementation & p_evaluation);

  /** Parameter constructor */
  InverseTrendTransform(const NumericalMathEvaluationImplementation & evaluation);

  /** Virtual constructor */
  virtual InverseTrendTransform * clone() const;

  /** Inverse accessor */
  TrendTransform getInverse() const;

  /** Comparison operator */
  Bool operator ==(const InverseTrendTransform & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** Operator () */
  using TemporalFunction::operator();
  TimeSeries operator() (const TimeSeries & inTS) const;


  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

}; /* class InverseTrendTransform */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_INVERSETRENDTRANSOFORM_HXX */
