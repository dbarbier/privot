//                                               -*- C++ -*-
/**
 *  @file  NumericalMathFunction.cxx
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
 *  @date   2012-07-16 12:24:33 +0200 (Mon, 16 Jul 2012)
 */
#include "NumericalMathFunction.hxx"
#include "ComposedNumericalMathFunction.hxx"
#include "AggregatedNumericalMathEvaluationImplementation.hxx"
#include "IndicatorNumericalMathEvaluationImplementation.hxx"
#include "DualLinearCombinationEvaluationImplementation.hxx"
#include "DualLinearCombinationGradientImplementation.hxx"
#include "DualLinearCombinationHessianImplementation.hxx"
#include "LinearCombinationEvaluationImplementation.hxx"
#include "LinearCombinationGradientImplementation.hxx"
#include "LinearCombinationHessianImplementation.hxx"
#include "NoNumericalMathGradientImplementation.hxx"
#include "NoNumericalMathHessianImplementation.hxx"
#include "Log.hxx"
#include "Os.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(NumericalMathFunction);


/* Default constructor */
NumericalMathFunction::NumericalMathFunction()
  : TypedInterfaceObject<NumericalMathFunctionImplementation>(new NumericalMathFunctionImplementation())
{
  // Nothing to do
}

/* Default constructor */
NumericalMathFunction::NumericalMathFunction(const String & name)
  : TypedInterfaceObject<NumericalMathFunctionImplementation>(new NumericalMathFunctionImplementation(name))
{
  // Nothing to do
}

/* Constructor from NumericalMathFunctionImplementation */
NumericalMathFunction::NumericalMathFunction(const NumericalMathFunctionImplementation & implementation)
  : TypedInterfaceObject<NumericalMathFunctionImplementation>(implementation.clone())
{
  // Nothing to do
}

/* Constructor from implementation */
NumericalMathFunction::NumericalMathFunction(const Implementation & p_implementation)
  : TypedInterfaceObject<NumericalMathFunctionImplementation>(p_implementation)
{
  // Nothing to do
}

/* Constructor from implementation pointer */
NumericalMathFunction::NumericalMathFunction(NumericalMathFunctionImplementation * p_implementation)
  : TypedInterfaceObject<NumericalMathFunctionImplementation>(p_implementation)
{
  // Nothing to do
}


/* Constructor from implementation pointer */
NumericalMathFunction::NumericalMathFunction(const NumericalMathEvaluationImplementation & evaluation)
  : TypedInterfaceObject<NumericalMathFunctionImplementation>(new NumericalMathFunctionImplementation(evaluation.clone()))
{
  // Nothing to do
}


/* Composition constructor */
NumericalMathFunction::NumericalMathFunction(const NumericalMathFunction & left,
                                             const NumericalMathFunction & right)
  : TypedInterfaceObject<NumericalMathFunctionImplementation>(new ComposedNumericalMathFunction(left.getImplementation(), right.getImplementation()))
{
  // Nothing to do
}

/* Analytical formula constructor */
NumericalMathFunction::NumericalMathFunction(const Description & inputVariablesNames,
                                             const Description & outputVariablesNames,
                                             const Description & formulas)
  : TypedInterfaceObject<NumericalMathFunctionImplementation>(new NumericalMathFunctionImplementation(inputVariablesNames, outputVariablesNames, formulas))
{
  // Nothing to do
}

/* Analytical formula constructor */
NumericalMathFunction::NumericalMathFunction(const Description & inputVariablesNames,
                                             const Description & formulas)
  : TypedInterfaceObject<NumericalMathFunctionImplementation>()
{
  const UnsignedLong size(formulas.getSize());
  Description outputVariablesNames(size);
  for (UnsignedLong i = 0; i < size; ++i)
    outputVariablesNames[i] = String(OSS() << "y" << i);
  *this = NumericalMathFunction(inputVariablesNames, outputVariablesNames, formulas);
}

/* Indicator function constructor */
NumericalMathFunction::NumericalMathFunction(const NumericalMathFunction & function,
                                             const ComparisonOperator & comparisonOperator,
                                             const NumericalScalar threshold)
  : TypedInterfaceObject<NumericalMathFunctionImplementation>(new NumericalMathFunctionImplementation(new IndicatorNumericalMathEvaluationImplementation(function.getEvaluationImplementation(), comparisonOperator, threshold), new NoNumericalMathGradientImplementation(), new NoNumericalMathHessianImplementation()))
{
  // Nothing to do
}

/* Aggregated function constructor: the output is the aggregation of the several functions */
NumericalMathFunction::NumericalMathFunction(const NumericalMathFunctionCollection & functionCollection)
  : TypedInterfaceObject<NumericalMathFunctionImplementation>(new NumericalMathFunctionImplementation(AggregatedNumericalMathEvaluationImplementation(functionCollection).clone()))
{
  // Nothing to do
}

/* Linear combination function constructor */
NumericalMathFunction::NumericalMathFunction(const NumericalMathFunctionCollection & functionCollection,
                                             const NumericalPoint & coefficients)
  : TypedInterfaceObject<NumericalMathFunctionImplementation>(new NumericalMathFunctionImplementation())
{
  LinearCombinationEvaluationImplementation evaluation(functionCollection, coefficients);
  setEvaluationImplementation(evaluation.clone());
  setGradientImplementation(new LinearCombinationGradientImplementation(evaluation));
  setHessianImplementation(new LinearCombinationHessianImplementation(evaluation));
}

/* Dual linear combination function constructor */
NumericalMathFunction::NumericalMathFunction(const NumericalMathFunctionCollection & functionCollection,
                                             const NumericalSample & coefficients)
  : TypedInterfaceObject<NumericalMathFunctionImplementation>(new NumericalMathFunctionImplementation())
{
  DualLinearCombinationEvaluationImplementation evaluation(functionCollection, coefficients);
  setEvaluationImplementation(evaluation.clone());
  setGradientImplementation(new DualLinearCombinationGradientImplementation(evaluation));
  setHessianImplementation(new DualLinearCombinationHessianImplementation(evaluation));
}

/* Simplified analytical formula constructor */
NumericalMathFunction::NumericalMathFunction(const String & inputVariableName,
                                             const String & formula,
                                             const String & outputVariableName)
  : TypedInterfaceObject<NumericalMathFunctionImplementation>(new NumericalMathFunctionImplementation(Description(1, inputVariableName), Description(1, outputVariableName), Description(1, formula)))
{
  // Nothing to do
}

/* Constructor from implementations */
NumericalMathFunction::NumericalMathFunction(const EvaluationImplementation & evaluationImplementation,
                                             const GradientImplementation & gradientImplementation,
                                             const HessianImplementation  & hessianImplementation)
  : TypedInterfaceObject<NumericalMathFunctionImplementation>(new NumericalMathFunctionImplementation(evaluationImplementation, gradientImplementation, hessianImplementation))
{
  // Nothing to do
}

/* Constructor from a wrapper file */
NumericalMathFunction::NumericalMathFunction(const WrapperFile & wrapperFile)
  : TypedInterfaceObject<NumericalMathFunctionImplementation>(new NumericalMathFunctionImplementation(wrapperFile))
{
  // Nothing to do
}

/* Constructor from samples */
NumericalMathFunction::NumericalMathFunction(const NumericalSample & inputSample,
                                             const NumericalSample & outputSample)
  : TypedInterfaceObject<NumericalMathFunctionImplementation>(new NumericalMathFunctionImplementation( inputSample, outputSample ))
{
  // Nothing to do
}

/* Comparison operator */
Bool NumericalMathFunction::operator ==(const NumericalMathFunction & other) const
{
  return true;
}

/* String converter */
String NumericalMathFunction::__repr__() const
{
  return OSS() << "class=" << NumericalMathFunction::GetClassName()
               << " name=" << getName()
               << " implementation=" << getImplementation()->__repr__();
}

/* String converter */
String NumericalMathFunction::__str__(const String & offset) const
{
  return OSS(false) << offset << getClassName() << " :\n" << getImplementation()->__str__( offset + "  ");
}

/* Description Accessor */
void NumericalMathFunction::setDescription(const Description & description)
{
  copyOnWrite();
  getImplementation()->setDescription(description);
}

/* Description Accessor */
Description NumericalMathFunction::getDescription() const
{
  return getImplementation()->getDescription();
}

/* Input description Accessor */
Description NumericalMathFunction::getInputDescription() const
{
  return getImplementation()->getInputDescription();
}

/* Output description Accessor */
Description NumericalMathFunction::getOutputDescription() const
{
  return getImplementation()->getOutputDescription();
}

/* Enable or disable the internal cache */
void NumericalMathFunction::enableCache() const
{
  getImplementation()->enableCache();
}

void NumericalMathFunction::disableCache() const
{
  getImplementation()->disableCache();
}

Bool NumericalMathFunction::isCacheEnabled() const
{
  return getImplementation()->isCacheEnabled();
}

UnsignedLong NumericalMathFunction::getCacheHits() const
{
  return getImplementation()->getCacheHits();
}

void NumericalMathFunction::addCacheContent(const NumericalSample & inSample, const NumericalSample & outSample)
{
  getImplementation()->addCacheContent(inSample, outSample);
}

NumericalSample NumericalMathFunction::getCacheInput() const
{
  return getImplementation()->getCacheInput();
}

NumericalSample NumericalMathFunction::getCacheOutput() const
{
  return getImplementation()->getCacheOutput();
}

void NumericalMathFunction::clearCache() const
{
  return getImplementation()->clearCache();
}

/* Enable or disable the input/output history */
void NumericalMathFunction::enableHistory() const
{
  return getImplementation()->enableHistory();
}

void NumericalMathFunction::disableHistory() const
{
  return getImplementation()->disableHistory();
}

Bool NumericalMathFunction::isHistoryEnabled() const
{
  return getImplementation()->isHistoryEnabled();
}

void NumericalMathFunction::clearHistory() const
{
  return getImplementation()->clearHistory();
}

void NumericalMathFunction::resetHistory() const
{
  return getImplementation()->resetHistory();
}

HistoryStrategy NumericalMathFunction::getInputHistory() const
{
  return getImplementation()->getInputHistory();
}

HistoryStrategy NumericalMathFunction::getOutputHistory() const
{
  return getImplementation()->getOutputHistory();
}

/* Multiplication operator between two functions with the same input dimension and 1D output dimension */
NumericalMathFunction NumericalMathFunction::operator * (const NumericalMathFunction & right) const
{
  return getImplementation()->operator * (*(right.getImplementation()));
}

/* Function implementation accessors */
void NumericalMathFunction::setEvaluationImplementation(const EvaluationImplementation & functionImplementation)
{
  copyOnWrite();
  getImplementation()->setEvaluationImplementation(functionImplementation);
}

/* Function implementation accessors */
const NumericalMathFunction::EvaluationImplementation & NumericalMathFunction::getEvaluationImplementation() const
{
  return getImplementation()->getEvaluationImplementation();
}


/* Gradient implementation accessors */
void NumericalMathFunction::setGradientImplementation(const NumericalMathGradientImplementation & gradientImplementation)
{
  copyOnWrite();
  getImplementation()->setGradientImplementation(GradientImplementation(gradientImplementation.clone()));
}

/* Gradient implementation accessors */
void NumericalMathFunction::setGradientImplementation(const GradientImplementation & gradientImplementation)
{
  copyOnWrite();
  getImplementation()->setGradientImplementation(gradientImplementation);
}

/* Gradient implementation accessors */
const NumericalMathFunction::GradientImplementation & NumericalMathFunction::getGradientImplementation() const
{
  return getImplementation()->getGradientImplementation();
}


/* Hessian implementation accessors */
void NumericalMathFunction::setHessianImplementation(const NumericalMathHessianImplementation & hessianImplementation)
{
  copyOnWrite();
  getImplementation()->setHessianImplementation(HessianImplementation(hessianImplementation.clone()));
}

void NumericalMathFunction::setHessianImplementation(const HessianImplementation & hessianImplementation)
{
  copyOnWrite();
  getImplementation()->setHessianImplementation(hessianImplementation);
}

/* Hessian implementation accessors */
const NumericalMathFunction::HessianImplementation & NumericalMathFunction::getHessianImplementation() const
{
  return getImplementation()->getHessianImplementation();
}

/* Initial Function implementation accessors */
const NumericalMathFunction::EvaluationImplementation & NumericalMathFunction::getInitialEvaluationImplementation() const
{
  return getImplementation()->getInitialEvaluationImplementation();
}

/* Initial gradient implementation accessors */
const NumericalMathFunction::GradientImplementation & NumericalMathFunction::getInitialGradientImplementation() const
{
  return getImplementation()->getInitialGradientImplementation();
}

/* Initial hessian implementation accessors */
const NumericalMathFunction::HessianImplementation & NumericalMathFunction::getInitialHessianImplementation() const
{
  return getImplementation()->getInitialHessianImplementation();
}

/* Flag for default gradient accessors */
Bool NumericalMathFunction::getUseDefaultGradientImplementation() const
{
  return getImplementation()->getUseDefaultGradientImplementation();
}

void NumericalMathFunction::setUseDefaultGradientImplementation(const Bool gradientFlag)
{
  copyOnWrite();
  getImplementation()->setUseDefaultGradientImplementation(gradientFlag);
}

/* Flag for default hessian accessors */
Bool NumericalMathFunction::getUseDefaultHessianImplementation() const
{
  return getImplementation()->getUseDefaultHessianImplementation();
}

void NumericalMathFunction::setUseDefaultHessianImplementation(const Bool hessianFlag)
{
  copyOnWrite();
  getImplementation()->setUseDefaultHessianImplementation(hessianFlag);
}


/* Gradient according to the marginal parameters */
Matrix NumericalMathFunction::parametersGradient(const NumericalPoint & inP) const
{
  return getImplementation()->parametersGradient(inP);
}

/* Parameters value and description accessor */
NumericalPointWithDescription NumericalMathFunction::getParameters() const
{
  return getImplementation()->getParameters();
}

void NumericalMathFunction::setParameters(const NumericalPointWithDescription & parameters)
{
  copyOnWrite();
  getImplementation()->setParameters(parameters);
}


/* Operator () */
NumericalPoint NumericalMathFunction::operator() (const NumericalPoint & inP) const
{
  return getImplementation()->operator()(inP);
}

/* Operator () */
NumericalSample NumericalMathFunction::operator() (const NumericalSample & inSample) const
{
  return getImplementation()->operator()(inSample);
}

/* Operator () */
TimeSeries NumericalMathFunction::operator() (const TimeSeries & inTimeSeries) const
{
  return getImplementation()->operator()(inTimeSeries);
}

/* Method gradient() returns the Jacobian transposed matrix of the function at point */
Matrix NumericalMathFunction::gradient(const NumericalPoint & inP) const
{
  return getImplementation()->gradient(inP);
}

/* Method hessian() returns the symmetric tensor of the function at point */
SymmetricTensor NumericalMathFunction::hessian(const NumericalPoint & inP) const
{
  return getImplementation()->hessian(inP);
}




/* Accessor for input point dimension */
UnsignedLong NumericalMathFunction::getInputDimension() const
{
  return getImplementation()->getInputDimension();
}

/* Accessor for output point dimension */
UnsignedLong NumericalMathFunction::getOutputDimension() const
{
  return getImplementation()->getOutputDimension();

}

/* Get the i-th marginal function */
NumericalMathFunction NumericalMathFunction::getMarginal(const UnsignedLong i) const
{
  return *(getImplementation()->getMarginal(i));
}

/* Get the function corresponding to indices components */
NumericalMathFunction NumericalMathFunction::getMarginal(const Indices & indices) const
{
  return *(getImplementation()->getMarginal(indices));
}

/* Number of calls to the evaluation */
UnsignedLong NumericalMathFunction::getEvaluationCallsNumber() const
{
  return getImplementation()->getEvaluationCallsNumber();
}

/* Number of calls to the gradient */
UnsignedLong NumericalMathFunction::getGradientCallsNumber() const
{
  return getImplementation()->getGradientCallsNumber();
}

/* Number of calls to the hessian */
UnsignedLong NumericalMathFunction::getHessianCallsNumber() const
{
  return getImplementation()->getHessianCallsNumber();
}

/* Draw the given 1D marginal output as a function of the given 1D marginal input around the given central point */
Graph NumericalMathFunction::draw(const UnsignedLong inputMarginal,
                                  const UnsignedLong outputMarginal,
                                  const NumericalPoint & centralPoint,
                                  const NumericalScalar xMin,
                                  const NumericalScalar xMax,
                                  const UnsignedLong pointNumber) const
{
  return getImplementation()->draw(inputMarginal, outputMarginal, centralPoint, xMin, xMax, pointNumber);
}

/* Draw the given 1D marginal output as a function of the given 1D marginal input around the given central point */
Graph NumericalMathFunction::draw(const UnsignedLong firstInputMarginal,
                                  const UnsignedLong secondInputMarginal,
                                  const UnsignedLong outputMarginal,
                                  const NumericalPoint & centralPoint,
                                  const NumericalPoint & xMin,
                                  const NumericalPoint & xMax,
                                  const Indices & pointNumber) const
{
  return getImplementation()->draw(firstInputMarginal, secondInputMarginal, outputMarginal, centralPoint, xMin, xMax, pointNumber);
}

/* Draw the output of the function with respect to its input when the input and output dimensions are 1 */
Graph NumericalMathFunction::draw(const NumericalScalar xMin,
                                  const NumericalScalar xMax,
                                  const UnsignedLong pointNumber) const
{
  return getImplementation()->draw(xMin, xMax, pointNumber);
}

/* Draw the output of the function with respect to its input when the input dimension is 2 and the output dimension is 1 */
Graph NumericalMathFunction::draw(const NumericalPoint & xMin,
                                  const NumericalPoint & xMax,
                                  const Indices & pointNumber) const
{
  return getImplementation()->draw(xMin, xMax, pointNumber);
}

/* Static methods for documentation of analytical fnctions */
Description NumericalMathFunction::GetValidConstants()
{
  return NumericalMathFunctionImplementation::GetValidConstants();
}

Description NumericalMathFunction::GetValidFunctions()
{
  return NumericalMathFunctionImplementation::GetValidFunctions();
}

Description NumericalMathFunction::GetValidOperators()
{
  return NumericalMathFunctionImplementation::GetValidOperators();
}



END_NAMESPACE_OPENTURNS
