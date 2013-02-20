//                                               -*- C++ -*-
/**
 *  @file  TemporalFunction.cxx
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
 *  @date   2011-05-24 19:30:41 +0200 (mar. 24 mai 2011)
 */
#include "TemporalFunction.hxx"
#include "PersistentObjectFactory.hxx"
#include "NumericalMathEvaluationImplementation.hxx"
#include "NoNumericalMathEvaluationImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(TemporalFunction);

static Factory<TemporalFunction> RegisteredFactory("TemporalFunction");

/* Default constructor */
TemporalFunction::TemporalFunction()
  : DynamicalFunctionImplementation(),
    p_evaluation_(new NoNumericalMathEvaluationImplementation)
{
  // Nothing to do
}

/* Parameter constructor */
TemporalFunction::TemporalFunction(const NumericalMathFunction & function)
  : DynamicalFunctionImplementation(),
    p_evaluation_(function.getEvaluationImplementation())
{
  // Set the descriptions
  DescriptionImplementation inputDescription(*(p_evaluation_->getInputDescription().getImplementation()));
  inputDescription.erase(inputDescription.begin(), inputDescription.begin() + 1);
  setInputDescription(inputDescription);
  setOutputDescription(p_evaluation_->getOutputDescription());
}

/* Parameter constructor */
TemporalFunction::TemporalFunction(const EvaluationImplementation & p_evaluation)
  : DynamicalFunctionImplementation(),
    p_evaluation_(p_evaluation)
{
  // Set the descriptions
  DescriptionImplementation inputDescription(*(p_evaluation_->getInputDescription().getImplementation()));
  inputDescription.erase(inputDescription.begin(), inputDescription.begin() + 1);
  setInputDescription(inputDescription);
  setOutputDescription(p_evaluation_->getOutputDescription());
}

/* Parameter constructor */
TemporalFunction::TemporalFunction(const NumericalMathEvaluationImplementation & evaluation)
  : DynamicalFunctionImplementation(),
    p_evaluation_(evaluation.clone())
{
  // Set the descriptions
  DescriptionImplementation inputDescription(*(p_evaluation_->getInputDescription().getImplementation()));
  inputDescription.erase(inputDescription.begin(), inputDescription.begin() + 1);
  setInputDescription(inputDescription);
  setOutputDescription(p_evaluation_->getOutputDescription());
}

/* Virtual constructor */
TemporalFunction * TemporalFunction::clone() const
{
  return new TemporalFunction(*this);
}

/* Comparison operator */
Bool TemporalFunction::operator ==(const TemporalFunction & other) const
{
  return true;
}

/* String converter */
String TemporalFunction::__repr__() const
{
  OSS oss;
  oss << "class=" << TemporalFunction::GetClassName()
      << " evaluation=" << p_evaluation_->__repr__();
  return oss;
}

/* String converter */
String TemporalFunction::__str__(const String & offset) const
{
  return OSS(false) << p_evaluation_->__str__(offset);
}

/* Operator () */
TimeSeries TemporalFunction::operator() (const TimeSeries & inTS) const
{
  ++callsNumber_;
  return TimeSeries(inTS.getTimeGrid(), (*p_evaluation_)(inTS.getImplementation()->asSample()));
}

/* Get the i-th marginal function */
TemporalFunction::Implementation TemporalFunction::getMarginal(const UnsignedLong i) const
{
  if (i >= getOutputDimension()) throw InvalidArgumentException(HERE) << "Error: the index of a marginal function must be in the range [0, outputDimension-1]";
  return new TemporalFunction(p_evaluation_->getMarginal(i));
}

/* Get the function corresponding to indices components */
TemporalFunction::Implementation TemporalFunction::getMarginal(const Indices & indices) const
{
  if (!indices.check(getOutputDimension() - 1)) throw InvalidArgumentException(HERE) << "Error: the indices of a marginal function must be in the range [0, outputDimension-1] and  must be different";
  return new TemporalFunction(p_evaluation_->getMarginal(indices));
}

/* Method save() stores the object through the StorageManager */
void TemporalFunction::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "evaluation_", *p_evaluation_ );
}

/* Evaluation accessor */
TemporalFunction::EvaluationImplementation TemporalFunction::getEvaluation() const
{
  return p_evaluation_;
}

/* Method load() reloads the object from the StorageManager */
void TemporalFunction::load(Advocate & adv)
{
  TypedInterfaceObject<NumericalMathEvaluationImplementation> evaluationValue;
  PersistentObject::load(adv);
  adv.loadAttribute( "evaluation_", evaluationValue );
  p_evaluation_ = evaluationValue.getImplementation();
}




END_NAMESPACE_OPENTURNS
