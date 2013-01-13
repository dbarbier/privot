//                                               -*- C++ -*-
/**
 *  @file  TemporalFunction.hxx
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
 *  @date   2011-04-11 12:32:27 +0200 (lun. 11 avril 2011)
 *  Id      TemporalFunction.hxx 1866 2011-04-11 10:32:27Z schueller
 */
#ifndef OPENTURNS_TEMPORALFUNCTION_HXX
#define OPENTURNS_TEMPORALFUNCTION_HXX

#include "PersistentObject.hxx"
#include "DynamicalFunctionImplementation.hxx"
#include "NumericalMathEvaluationImplementation.hxx"
#include "NumericalMathFunction.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class TemporalFunction
 *
 * The class that simulates a dynamical function based on a
 * numerical math function that acts only on the temporal part
 * of a time series.
 */
class TemporalFunction
  : public DynamicalFunctionImplementation
{
  CLASSNAME;
public:

  typedef NumericalMathEvaluationImplementation::Implementation EvaluationImplementation;

  /** Default constructor */
  TemporalFunction();

  /** Parameter constructor */
  TemporalFunction(const NumericalMathFunction & function);

#ifndef SWIG
  /** Parameter constructor */
  TemporalFunction(const EvaluationImplementation & p_evaluation);
#endif

  /** Parameter constructor */
  TemporalFunction(const NumericalMathEvaluationImplementation & evaluation);

  /** Virtual constructor */
  virtual TemporalFunction * clone() const;

  /** Comparison operator */
  Bool operator ==(const TemporalFunction & other) const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Operator () */
  TimeSeries operator() (const TimeSeries & inTS) const;

  /** Get the i-th marginal function */
  Implementation getMarginal(const UnsignedLong i) const;

  /** Get the function corresponding to indices components */
  Implementation getMarginal(const Indices & indices) const;

  /** Evaluation accessor */
  EvaluationImplementation getEvaluation() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

  /** Pointer to the actual evaluation class */
  EvaluationImplementation p_evaluation_;

}; /* class TemporalFunction */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_TEMPORALFUNCTION_HXX */
