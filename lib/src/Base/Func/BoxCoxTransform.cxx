//                                               -*- C++ -*-
/**
 *  @file  BoxCoxTransform.cxx
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
#include "BoxCoxTransform.hxx"
#include "PersistentObjectFactory.hxx"
#include "BoxCoxEvaluationImplementation.hxx"
#include "InverseBoxCoxTransform.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(BoxCoxTransform);

static Factory<BoxCoxTransform> RegisteredFactory("BoxCoxTransform");

/* Default constructor */
BoxCoxTransform::BoxCoxTransform()
  : SpatialFunction(BoxCoxEvaluationImplementation())
{
  setDefaultDescription();
}

/* Standard parameter constructor */
BoxCoxTransform::BoxCoxTransform(const NumericalPoint & lambdaPoint)
  : SpatialFunction(),
    lambda_(lambdaPoint)
{
  const BoxCoxEvaluationImplementation evaluation(lambda_);
  p_evaluation_ = evaluation.clone();
  setDefaultDescription();
}

/* NumericalScalarCollection parameter constructor */
BoxCoxTransform::BoxCoxTransform(const Collection<NumericalScalar> & lambdaCollection)
  : SpatialFunction(),
    lambda_(NumericalPoint(lambdaCollection))
{
  const BoxCoxEvaluationImplementation evaluation(lambda_);
  p_evaluation_ = evaluation.clone();
  setDefaultDescription();
}

/* 1D NumericalScalar parameter constructor */
BoxCoxTransform::BoxCoxTransform(const NumericalScalar & lambdaScalar)
  : SpatialFunction(),
    lambda_(NumericalPoint(1, lambdaScalar))
{
  const BoxCoxEvaluationImplementation evaluation(lambda_);
  p_evaluation_ = evaluation.clone();
  setDefaultDescription();
}

/* Virtual constructor */
BoxCoxTransform * BoxCoxTransform::clone() const
{
  return new BoxCoxTransform(*this);
}

/* Comparison operator */
Bool BoxCoxTransform::operator ==(const BoxCoxTransform & other) const
{
  return true;
}

/* String converter */
String BoxCoxTransform::__repr__() const
{
  OSS oss;
  oss << "class=" << BoxCoxTransform::GetClassName()
      << " evaluation=" << p_evaluation_->__repr__();
  return oss;
}

/* String converter */
String BoxCoxTransform::__str__(const String & offset) const
{
  return OSS(false) << p_evaluation_->__str__(offset);
}

/* Evaluation accessor */
BoxCoxTransform::EvaluationImplementation BoxCoxTransform::getEvaluation() const
{
  return p_evaluation_;
}

void BoxCoxTransform::setDefaultDescription()
{
  const UnsignedLong size(lambda_.getSize());
  Description description(size);
  for (UnsignedLong k = 0; k < size; ++k)
    {
      description[k] =  String(OSS() << "Marginal " << k);  ;
    }
  // call DynamicalFunction description accessors
  setInputDescription(description);
  setOutputDescription(description);
}

/* Lambda accessor */
NumericalPoint BoxCoxTransform::getLambda() const
{
  return lambda_;
}

/* Operator () */
TimeSeries BoxCoxTransform::operator() (const TimeSeries & inTS) const
{

  // Before applying the function to the TimeSeries, we check that all values are > 0
  // otherwise we raise an exception
  NumericalSample sampleValues(inTS.getSample());

  // Get the min point of the sample and translate if needed
  NumericalPoint minValueTranslate (inTS.getSample().getMin());
  for (UnsignedLong n = 0; n < minValueTranslate.getDimension(); ++n)
    {
      if (minValueTranslate[n] <= 0)
        throw InvalidArgumentException(HERE) << "Can not apply the Box Cox function to negative values" ;
    }
  const NumericalSample result((*p_evaluation_)(sampleValues));
  ++callsNumber_;
  return TimeSeries(inTS.getTimeGrid(), result);
}

/* Inverse accessor */
InverseBoxCoxTransform BoxCoxTransform::getInverse() const
{
  return InverseBoxCoxTransform(lambda_);
}

/* Method save() stores the object through the StorageManager */
void BoxCoxTransform::save(Advocate & adv) const
{
  SpatialFunction::save(adv);
  adv.saveAttribute( "lamdba_", lambda_ );
}

/* Method load() reloads the object from the StorageManager */
void BoxCoxTransform::load(Advocate & adv)
{
  SpatialFunction::load(adv);
  adv.loadAttribute( "lamdba_", lambda_ );
}

END_NAMESPACE_OPENTURNS
