//                                               -*- C++ -*-
/**
 *  @file  TrendTransform.cxx
 *  @brief Abstract top-level class for all distributions
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 */
#include "TrendTransform.hxx"
#include "InverseTrendTransform.hxx"
#include "PersistentObjectFactory.hxx"
#include "NumericalMathEvaluationImplementation.hxx"
#include "NumericalSample.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(TrendTransform);

static Factory<TrendTransform> RegisteredFactory("TrendTransform");

/* Default constructor */
TrendTransform::TrendTransform()
  : TemporalFunction()
{
  // Nothing to do
}

/* Parameter constructor */
TrendTransform::TrendTransform(const NumericalMathFunction & function)
  : TemporalFunction()
{
  if (function.getInputDimension() != 1)
    throw InvalidDimensionException(HERE) << "NumericalMathFunction input dimension should be 1 for trend" ;
  p_evaluation_ = function.getEvaluationImplementation() ;
  // Set the descriptions
  setInputDescription(p_evaluation_->getInputDescription());
  setOutputDescription(p_evaluation_->getOutputDescription());
}

/* Parameter constructor */
TrendTransform::TrendTransform(const EvaluationImplementation & p_evaluation)
  : TemporalFunction()
{
  if (p_evaluation->getInputDimension() != 1)
    throw InvalidDimensionException(HERE) << "NumericalMathFunction input dimension should be 1 for trend" ;
  p_evaluation_ = p_evaluation;
  // Set the descriptions
  setInputDescription(p_evaluation_->getInputDescription());
  setOutputDescription(p_evaluation_->getOutputDescription());
}

/* Parameter constructor */
TrendTransform::TrendTransform(const NumericalMathEvaluationImplementation & evaluation)
  : TemporalFunction()
{
  if (evaluation.getInputDimension() != 1)
    throw InvalidDimensionException(HERE) << "NumericalMathFunction input dimension should be 1 for trend" ;
  p_evaluation_ = evaluation.clone();
}

/* Virtual constructor */
TrendTransform * TrendTransform::clone() const
{
  return new TrendTransform(*this);
}

/* Comparison operator */
Bool TrendTransform::operator ==(const TrendTransform & other) const
{
  return (getEvaluation() == other.getEvaluation());
}

/* String converter */
String TrendTransform::__repr__() const
{
  OSS oss;
  oss << "class=" << TrendTransform::GetClassName()
      << " evaluation=" << p_evaluation_->__repr__();
  return oss;
}

/* String converter */
String TrendTransform::__str__(const String & offset) const
{
  return OSS(false) << p_evaluation_->__str__(offset);
}

/* Operator () */
TimeSeries TrendTransform::operator() (const TimeSeries & inTS) const
{
  // the considered \phi function should from from R to R^n
  // the check will be done later
  // since the TimeSeriesImplementation is not available, we have to check some features carefully

  // 0 : Rework the timeSeries ==> for huge timeSeries, it is better to extract information from the ts
  // without using the asNumericalSample method
  // first we extract the timeGrid as a NumericalSample
  NumericalSample timeGridAsSample(inTS.getSize() , 1);

  for (UnsignedLong k = 0; k < timeGridAsSample.getSize(); ++k)
    {
      timeGridAsSample[k][0] = inTS[k][0];
    }

  // then we evaluation the function on the previous grid
  // result is a NumericalSample of dimension d = function.getOutputDimension()
  NumericalSample outputSample((*p_evaluation_)(timeGridAsSample));
  const UnsignedLong dimension(outputSample.getDimension());
  // finally as the function adds a trend, result
  for (UnsignedLong k = 0; k < timeGridAsSample.getSize(); ++k)
    {
      for (UnsignedLong d = 0; d < dimension; ++d)
        {
          // NumericalPoint should be compatible
          outputSample[k][d] += inTS[k][d + 1];
        }
    }
  ++callsNumber_;
  return TimeSeries(inTS.getTimeGrid(), outputSample);
}

/* Inverse accessor */
InverseTrendTransform TrendTransform::getInverse() const
{
  return InverseTrendTransform(p_evaluation_);
}

/* Method save() stores the object through the StorageManager */
void TrendTransform::save(Advocate & adv) const
{
  TemporalFunction::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void TrendTransform::load(Advocate & adv)
{
  TemporalFunction::load(adv);
}

END_NAMESPACE_OPENTURNS
