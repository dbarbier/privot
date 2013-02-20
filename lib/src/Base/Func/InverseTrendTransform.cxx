//                                               -*- C++ -*-
/**
 *  @file  InverseTrendTransform.cxx
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
 *  @author lebrun
 *  @date   2011-12-19 23:37:21 +0100 (lun. 19 déc. 2011)
 */
#include "InverseTrendTransform.hxx"
#include "TrendTransform.hxx"
#include "PersistentObjectFactory.hxx"
#include "NumericalMathEvaluationImplementation.hxx"
#include "NumericalSample.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(InverseTrendTransform);

static Factory<InverseTrendTransform> RegisteredFactory("InverseTrendTransform");

/* Default constructor */
InverseTrendTransform::InverseTrendTransform()
  : TemporalFunction()
{
  // Nothing to do
}

/* Parameter constructor */
InverseTrendTransform::InverseTrendTransform(const NumericalMathFunction & function)
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
InverseTrendTransform::InverseTrendTransform(const EvaluationImplementation & p_evaluation)
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
InverseTrendTransform::InverseTrendTransform(const NumericalMathEvaluationImplementation & evaluation)
  : TemporalFunction()
{
  if (evaluation.getInputDimension() != 1)
    throw InvalidDimensionException(HERE) << "NumericalMathFunction input dimension should be 1 for trend" ;
  p_evaluation_ = evaluation.clone();
}

/* Virtual constructor */
InverseTrendTransform * InverseTrendTransform::clone() const
{
  return new InverseTrendTransform(*this);
}

/* Comparison operator */
Bool InverseTrendTransform::operator ==(const InverseTrendTransform & other) const
{
  return (getEvaluation() == other.getEvaluation());
}

/* Inverse accessor */
TrendTransform InverseTrendTransform::getInverse() const
{
  return TrendTransform(p_evaluation_);
}

/* String converter */
String InverseTrendTransform::__repr__() const
{
  OSS oss;
  oss << "class=" << InverseTrendTransform::GetClassName()
      << " evaluation=" << p_evaluation_->__repr__();
  return oss;
}

/* String converter */
String InverseTrendTransform::__str__(const String & offset) const
{
  return OSS(false) << p_evaluation_->__str__(offset);
}

/* Operator () */
TimeSeries InverseTrendTransform::operator() (const TimeSeries & inTS) const
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
          outputSample[k][d] = inTS[k][d + 1] - outputSample[k][d];
        }
    }
  ++callsNumber_;
  return TimeSeries(inTS.getTimeGrid(), outputSample);
}

/* Method save() stores the object through the StorageManager */
void InverseTrendTransform::save(Advocate & adv) const
{
  TemporalFunction::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void InverseTrendTransform::load(Advocate & adv)
{
  TemporalFunction::load(adv);
}

END_NAMESPACE_OPENTURNS
