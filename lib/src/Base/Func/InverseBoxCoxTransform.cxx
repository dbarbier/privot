//                                               -*- C++ -*-
/**
 *  @file  InverseBoxCoxTransform.cxx
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
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */
#include "InverseBoxCoxTransform.hxx"
#include "PersistentObjectFactory.hxx"
#include "InverseBoxCoxEvaluationImplementation.hxx"
#include "BoxCoxTransform.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(InverseBoxCoxTransform);

static Factory<InverseBoxCoxTransform> RegisteredFactory("InverseBoxCoxTransform");

/* Default constructor */
InverseBoxCoxTransform::InverseBoxCoxTransform()
  : SpatialFunction(InverseBoxCoxEvaluationImplementation())
{
  setDefaultDescription();
}

/* Standard parameter constructor */
InverseBoxCoxTransform::InverseBoxCoxTransform(const NumericalPoint & lambdaPoint)
  : SpatialFunction(),
    lambda_(lambdaPoint)
{
  const InverseBoxCoxEvaluationImplementation evaluation(lambda_);
  p_evaluation_ = evaluation.clone();
  setDefaultDescription();
}

/* NumericalScalarCollection parameter constructor */
InverseBoxCoxTransform::InverseBoxCoxTransform(const Collection<NumericalScalar> & lambdaCollection)
  : SpatialFunction(),
    lambda_(NumericalPoint(lambdaCollection))
{
  const InverseBoxCoxEvaluationImplementation evaluation(lambda_);
  p_evaluation_ = evaluation.clone();
  setDefaultDescription();
}

/* 1D NumericalScalar parameter constructor */
InverseBoxCoxTransform::InverseBoxCoxTransform(const NumericalScalar & lambdaScalar)
  : SpatialFunction(),
    lambda_(NumericalPoint(1, lambdaScalar))
{
  const InverseBoxCoxEvaluationImplementation evaluation(lambda_);
  p_evaluation_ = evaluation.clone();
  setDefaultDescription();
}

/* Virtual constructor */
InverseBoxCoxTransform * InverseBoxCoxTransform::clone() const
{
  return new InverseBoxCoxTransform(*this);
}

/* Comparison operator */
Bool InverseBoxCoxTransform::operator ==(const InverseBoxCoxTransform & other) const
{
  return true;
}

/* String converter */
String InverseBoxCoxTransform::__repr__() const
{
  OSS oss;
  oss << "class=" << InverseBoxCoxTransform::GetClassName()
      << " evaluation=" << p_evaluation_->__repr__();
  return oss;
}

/* String converter */
String InverseBoxCoxTransform::__str__(const String & offset) const
{
  return OSS(false) << p_evaluation_->__str__(offset);
}

/* Evaluation accessor */
InverseBoxCoxTransform::EvaluationImplementation InverseBoxCoxTransform::getEvaluation() const
{
  return p_evaluation_;
}

/* Inverse accessor */
BoxCoxTransform InverseBoxCoxTransform::getInverse() const
{
  return BoxCoxTransform(lambda_);
}

/* Lambda accessor */
NumericalPoint InverseBoxCoxTransform::getLambda() const
{
  return lambda_;
}

/* Operator () */
TimeSeries InverseBoxCoxTransform::operator() (const TimeSeries & inTS) const
{

  // Before applying the function to the TimeSeries, we check that all values are > 0
  // otherwise we raise an exception
  NumericalSample sampleValues(inTS.getSample());
  const NumericalSample result((*p_evaluation_)(sampleValues));
  ++callsNumber_;
  return TimeSeries(inTS.getTimeGrid(), result);
}

void InverseBoxCoxTransform::setDefaultDescription()
{
  const UnsignedLong size(lambda_.getSize());
  Description description(size);
  for (UnsignedLong k = 0; k < size; ++k)
    {
      description[k] =  String(OSS() << "Marginal " << k);
    }
  // call DynamicalFunction description accessors
  setInputDescription(description);
  setOutputDescription(description);
}

/* Method save() stores the object through the StorageManager */
void InverseBoxCoxTransform::save(Advocate & adv) const
{
  SpatialFunction::save(adv);
  adv.saveAttribute( "lamdba_", lambda_ );
}

/* Method load() reloads the object from the StorageManager */
void InverseBoxCoxTransform::load(Advocate & adv)
{
  SpatialFunction::load(adv);
  adv.loadAttribute( "lamdba_", lambda_ );
}

END_NAMESPACE_OPENTURNS
