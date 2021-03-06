//                                               -*- C++ -*-
/**
 *  @file  NumericalMathFunctionImplementation.cxx
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
#include "NumericalMathFunctionImplementation.hxx"
#include "NoNumericalMathEvaluationImplementation.hxx"
#include "NoNumericalMathGradientImplementation.hxx"
#include "NoNumericalMathHessianImplementation.hxx"
#include "ComputedNumericalMathEvaluationImplementation.hxx"
#include "ComputedNumericalMathGradientImplementation.hxx"
#include "ComputedNumericalMathHessianImplementation.hxx"
#include "AnalyticalNumericalMathEvaluationImplementation.hxx"
#include "AnalyticalNumericalMathGradientImplementation.hxx"
#include "AnalyticalNumericalMathHessianImplementation.hxx"
#include "DatabaseNumericalMathEvaluationImplementation.hxx"
#include "ProductNumericalMathFunction.hxx"
#include "CenteredFiniteDifferenceGradient.hxx"
#include "CenteredFiniteDifferenceHessian.hxx"
#include "PersistentObjectFactory.hxx"
#include "ResourceMap.hxx"
#include "Log.hxx"
#include "OSS.hxx"
#include "Os.hxx"

#undef GetClassName

BEGIN_NAMESPACE_OPENTURNS




CLASSNAMEINIT(NumericalMathFunctionImplementation);

static Factory<NumericalMathFunctionImplementation> RegisteredFactory("NumericalMathFunctionImplementation");

// Inline documentation for analytical functions
Bool NumericalMathFunctionImplementation::IsDocumentationInitialized_ = false;
Description NumericalMathFunctionImplementation::ValidConstants_;
Description NumericalMathFunctionImplementation::ValidFunctions_;
Description NumericalMathFunctionImplementation::ValidOperators_;

/* Default constructor */
NumericalMathFunctionImplementation::NumericalMathFunctionImplementation()
  : PersistentObject(),
    p_evaluationImplementation_(new NoNumericalMathEvaluationImplementation),
    p_gradientImplementation_(new NoNumericalMathGradientImplementation),
    p_hessianImplementation_(new NoNumericalMathHessianImplementation),
    p_initialEvaluationImplementation_(p_evaluationImplementation_),
    p_initialGradientImplementation_(p_gradientImplementation_),
    p_initialHessianImplementation_(p_hessianImplementation_),
    useDefaultGradientImplementation_(false),
    useDefaultHessianImplementation_(false)
{
  // Nothing to do
}

/* Constructor from a wrapper name */
NumericalMathFunctionImplementation::NumericalMathFunctionImplementation(const String & name)
  : PersistentObject(name),
    p_evaluationImplementation_(new NoNumericalMathEvaluationImplementation),
    p_gradientImplementation_(new NoNumericalMathGradientImplementation),
    p_hessianImplementation_(new NoNumericalMathHessianImplementation),
    p_initialEvaluationImplementation_(p_evaluationImplementation_),
    p_initialGradientImplementation_(p_gradientImplementation_),
    p_initialHessianImplementation_(p_hessianImplementation_),
    useDefaultGradientImplementation_(false),
    useDefaultHessianImplementation_(false)
{
  // Read the description file of the wrapper
  WrapperFile wrapperFile = WrapperFile::FindWrapperByName( name );

  // We set the implementations
  initImplementations( wrapperFile );
}

/* Constructor from a wrapper file */
NumericalMathFunctionImplementation::NumericalMathFunctionImplementation(const WrapperFile & wrapperFile)
  : PersistentObject(wrapperFile.getName()),
    p_evaluationImplementation_(new NoNumericalMathEvaluationImplementation),
    p_gradientImplementation_(new NoNumericalMathGradientImplementation),
    p_hessianImplementation_(new NoNumericalMathHessianImplementation),
    p_initialEvaluationImplementation_(p_evaluationImplementation_),
    p_initialGradientImplementation_(p_gradientImplementation_),
    p_initialHessianImplementation_(p_hessianImplementation_),
    useDefaultGradientImplementation_(false),
    useDefaultHessianImplementation_(false)
{
  // We set the implementations
  initImplementations( wrapperFile );
}


/* This method set the implementations with the values listed in the wrapper file */
void NumericalMathFunctionImplementation::initImplementations(const WrapperFile & wrapperFile)
{
  const String name = wrapperFile.getName();

  ComputedNumericalMathEvaluationImplementation * implementation = new ComputedNumericalMathEvaluationImplementation( name, wrapperFile );
  p_evaluationImplementation_.reset(implementation);
  try
    {
      p_gradientImplementation_.reset(new ComputedNumericalMathGradientImplementation( name, wrapperFile, implementation->getState() ));
    }
  catch (Exception & ex)
    {
      LOGDEBUG(OSS() << "" << ex.type() << " catched. Using CenteredFiniteDifferenceGradient for gradient");
      p_gradientImplementation_.reset(new CenteredFiniteDifferenceGradient(sqrt(ResourceMap::GetAsNumericalScalar( "CenteredFiniteDifferenceGradient-DefaultEpsilon" )), p_evaluationImplementation_));
      useDefaultGradientImplementation_ = true;
    }

  try
    {
      p_hessianImplementation_.reset(new ComputedNumericalMathHessianImplementation( name, wrapperFile, implementation->getState() ));
    }
  catch (Exception & ex)
    {
      LOGDEBUG(OSS() << "" << ex.type() << " catched. Using CenteredFiniteDifferenceHessian for hessian");
      p_hessianImplementation_.reset(new CenteredFiniteDifferenceHessian(sqrt(ResourceMap::GetAsNumericalScalar( "CenteredFiniteDifferenceHessian-DefaultEpsilon" )), p_evaluationImplementation_));
      useDefaultHessianImplementation_ = true;
    }
}


/* Analytical formula constructor */
NumericalMathFunctionImplementation::NumericalMathFunctionImplementation(const Description & inputVariablesNames,
                                                                         const Description & outputVariablesNames,
                                                                         const Description & formulas)
  : PersistentObject(),
    p_evaluationImplementation_(new NoNumericalMathEvaluationImplementation),
    p_gradientImplementation_(new NoNumericalMathGradientImplementation),
    p_hessianImplementation_(new NoNumericalMathHessianImplementation),
    p_initialEvaluationImplementation_(p_evaluationImplementation_),
    p_initialGradientImplementation_(p_gradientImplementation_),
    p_initialHessianImplementation_(p_hessianImplementation_),
    useDefaultGradientImplementation_(true),
    useDefaultHessianImplementation_(true)
{
  // Try to build an analytical gradient
  AnalyticalNumericalMathEvaluationImplementation evaluation(inputVariablesNames, outputVariablesNames, formulas);
  p_evaluationImplementation_ = evaluation.clone();
  try
    {
      p_gradientImplementation_ = new AnalyticalNumericalMathGradientImplementation(evaluation);
      useDefaultGradientImplementation_ = false;
    }
  catch(...)
    {
      LOGWARN("Cannot compute an analytical gradient, using finite differences instead.");
      p_gradientImplementation_ = new CenteredFiniteDifferenceGradient(ResourceMap::GetAsNumericalScalar( "CenteredFiniteDifferenceGradient-DefaultEpsilon" ), p_evaluationImplementation_);
    }
  p_initialGradientImplementation_ = p_gradientImplementation_;
  try
    {
      p_hessianImplementation_ = new AnalyticalNumericalMathHessianImplementation(evaluation);
      useDefaultHessianImplementation_ = false;
    }
  catch(...)
    {
      LOGWARN("Cannot compute an analytical hessian, using finite differences instead.");
      p_hessianImplementation_ = new CenteredFiniteDifferenceHessian(ResourceMap::GetAsNumericalScalar( "CenteredFiniteDifferenceHessian-DefaultEpsilon" ), p_evaluationImplementation_);
    }
  p_initialHessianImplementation_ = p_hessianImplementation_;
}


/* Constructor from a wrapper file */
NumericalMathFunctionImplementation::NumericalMathFunctionImplementation(const NumericalSample & inputSample,
                                                                         const NumericalSample & outputSample)
  : PersistentObject(),
    p_gradientImplementation_(new NoNumericalMathGradientImplementation),
    p_hessianImplementation_(new NoNumericalMathHessianImplementation),
    p_initialGradientImplementation_(p_gradientImplementation_),
    p_initialHessianImplementation_(p_hessianImplementation_),
    useDefaultGradientImplementation_(false),
    useDefaultHessianImplementation_(false)
{
  p_evaluationImplementation_ = new DatabaseNumericalMathEvaluationImplementation( inputSample, outputSample );
  p_initialEvaluationImplementation_ = p_evaluationImplementation_;
}


/* Single function implementation constructor */
NumericalMathFunctionImplementation::NumericalMathFunctionImplementation(const EvaluationImplementation & evaluationImplementation)
  : PersistentObject(),
    p_evaluationImplementation_(evaluationImplementation),
    p_gradientImplementation_(new CenteredFiniteDifferenceGradient(ResourceMap::GetAsNumericalScalar( "CenteredFiniteDifferenceGradient-DefaultEpsilon" ), p_evaluationImplementation_)),
    p_hessianImplementation_(new CenteredFiniteDifferenceHessian(ResourceMap::GetAsNumericalScalar( "CenteredFiniteDifferenceGradient-DefaultEpsilon" ), p_evaluationImplementation_)),
    p_initialEvaluationImplementation_(p_evaluationImplementation_),
    p_initialGradientImplementation_(p_gradientImplementation_),
    p_initialHessianImplementation_(p_hessianImplementation_),
    useDefaultGradientImplementation_(true),
    useDefaultHessianImplementation_(true)
{
  // Nothing to do
}

/* Constructor from implementations */
NumericalMathFunctionImplementation::NumericalMathFunctionImplementation(const EvaluationImplementation & evaluationImplementation,
                                                                         const GradientImplementation & gradientImplementation,
                                                                         const HessianImplementation  & hessianImplementation)
  : PersistentObject(),
    p_evaluationImplementation_(evaluationImplementation),
    p_gradientImplementation_(gradientImplementation),
    p_hessianImplementation_(hessianImplementation),
    p_initialEvaluationImplementation_(p_evaluationImplementation_),
    p_initialGradientImplementation_(p_gradientImplementation_),
    p_initialHessianImplementation_(p_hessianImplementation_),
    useDefaultGradientImplementation_(false),
    useDefaultHessianImplementation_(false)
{
  // Nothing to do
}


/* Virtual constructor */
NumericalMathFunctionImplementation * NumericalMathFunctionImplementation::clone() const
{
  return new NumericalMathFunctionImplementation(*this);
}

/* Comparison operator */
Bool NumericalMathFunctionImplementation::operator ==(const NumericalMathFunctionImplementation & other) const
{
  return true;
}

/* String converter */
String NumericalMathFunctionImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << NumericalMathFunctionImplementation::GetClassName()
      << " name=" << getName()
      << " description=" << getDescription()
      << " evaluationImplementation=" << p_evaluationImplementation_->__repr__()
      << " gradientImplementation=" << p_gradientImplementation_->__repr__()
      << " hessianImplementation=" << p_hessianImplementation_->__repr__();
  return oss;
}

/* String converter */
String NumericalMathFunctionImplementation::__str__(const String & offset) const
{
  return OSS(false) << offset << "input  : " << getInputDescription() << Os::GetEndOfLine()
                    << offset << "output : " << getOutputDescription() << Os::GetEndOfLine()
                    << offset << "evaluation : " << p_evaluationImplementation_->__str__(offset) << Os::GetEndOfLine()
                    << offset << "gradient   : " << p_gradientImplementation_->__str__(offset) << Os::GetEndOfLine()
                    << offset << "hessian    : " << p_hessianImplementation_->__str__(offset);
}

/* Description Accessor */
void NumericalMathFunctionImplementation::setDescription(const Description & description)
{
  p_evaluationImplementation_->setDescription(description);
}

/* Description Accessor */
Description NumericalMathFunctionImplementation::getDescription() const
{
  return p_evaluationImplementation_->getDescription();
}

/* Input description Accessor */
Description NumericalMathFunctionImplementation::getInputDescription() const
{
  return p_evaluationImplementation_->getInputDescription();
}

/* Output description Accessor */
Description NumericalMathFunctionImplementation::getOutputDescription() const
{
  return p_evaluationImplementation_->getOutputDescription();
}

/* Enable or disable the internal cache */
void NumericalMathFunctionImplementation::enableCache() const
{
  p_evaluationImplementation_->enableCache();
}

void NumericalMathFunctionImplementation::disableCache() const
{
  p_evaluationImplementation_->disableCache();
}

Bool NumericalMathFunctionImplementation::isCacheEnabled() const
{
  return p_evaluationImplementation_->isCacheEnabled();
}

UnsignedLong NumericalMathFunctionImplementation::getCacheHits() const
{
  return p_evaluationImplementation_->getCacheHits();
}

void NumericalMathFunctionImplementation::addCacheContent(const NumericalSample& inSample, const NumericalSample& outSample)
{
  p_evaluationImplementation_->addCacheContent(inSample, outSample);
}

NumericalSample NumericalMathFunctionImplementation::getCacheInput() const
{
  return p_evaluationImplementation_->getCacheInput();
}

NumericalSample NumericalMathFunctionImplementation::getCacheOutput() const
{
  return p_evaluationImplementation_->getCacheOutput();
}

void NumericalMathFunctionImplementation::clearCache() const
{
  p_evaluationImplementation_->clearCache();
}

/* Enable or disable the input/output history */
void NumericalMathFunctionImplementation::enableHistory() const
{
  p_evaluationImplementation_->enableHistory();
}

void NumericalMathFunctionImplementation::disableHistory() const
{
  p_evaluationImplementation_->disableHistory();
}

Bool NumericalMathFunctionImplementation::isHistoryEnabled() const
{
  return p_evaluationImplementation_->isHistoryEnabled();
}

void NumericalMathFunctionImplementation::clearHistory() const
{
  p_evaluationImplementation_->clearHistory();
}

void NumericalMathFunctionImplementation::resetHistory() const
{
  p_evaluationImplementation_->resetHistory();
}

HistoryStrategy NumericalMathFunctionImplementation::getInputHistory() const
{
  return p_evaluationImplementation_->getInputHistory();
}

HistoryStrategy NumericalMathFunctionImplementation::getOutputHistory() const
{
  return p_evaluationImplementation_->getOutputHistory();
}

/* Multiplication operator between two functions with the same input dimension and 1D output dimension */
NumericalMathFunctionImplementation NumericalMathFunctionImplementation::operator * (const NumericalMathFunctionImplementation & right) const
{
  return ProductNumericalMathFunction(clone(), right.clone());
}

/* Multiplication operator between two functions with the same input dimension and 1D output dimension */
NumericalMathFunctionImplementation NumericalMathFunctionImplementation::operator * (const Implementation & p_right) const
{
  return ProductNumericalMathFunction(clone(), p_right);
}

/* Function implementation accessors */
void NumericalMathFunctionImplementation::setEvaluationImplementation(const EvaluationImplementation & evaluationImplementation)
{
  p_evaluationImplementation_ = evaluationImplementation;
}

/* Function implementation accessors */
const NumericalMathFunctionImplementation::EvaluationImplementation & NumericalMathFunctionImplementation::getEvaluationImplementation() const
{
  return p_evaluationImplementation_;
}


/* Gradient implementation accessors */
void NumericalMathFunctionImplementation::setGradientImplementation(const GradientImplementation & gradientImplementation)
{
  p_gradientImplementation_ = gradientImplementation;
  useDefaultGradientImplementation_ = false;
}

/* Gradient implementation accessors */
const NumericalMathFunctionImplementation::GradientImplementation & NumericalMathFunctionImplementation::getGradientImplementation() const
{
  return p_gradientImplementation_;
}


/* Hessian implementation accessors */
void NumericalMathFunctionImplementation::setHessianImplementation(const HessianImplementation & hessianImplementation)
{
  p_hessianImplementation_ = hessianImplementation;
  useDefaultHessianImplementation_ = false;
}

/* Hessian implementation accessors */
const NumericalMathFunctionImplementation::HessianImplementation & NumericalMathFunctionImplementation::getHessianImplementation() const
{
  return p_hessianImplementation_;
}

/* Initial Function implementation accessors */
const NumericalMathFunctionImplementation::EvaluationImplementation & NumericalMathFunctionImplementation::getInitialEvaluationImplementation() const
{
  return p_initialEvaluationImplementation_;
}
void NumericalMathFunctionImplementation::setInitialEvaluationImplementation(const EvaluationImplementation & initialEvaluationImplementation)
{
  p_evaluationImplementation_ = initialEvaluationImplementation;
}

/* Initial gradient implementation accessors */
const NumericalMathFunctionImplementation::GradientImplementation & NumericalMathFunctionImplementation::getInitialGradientImplementation() const
{
  return p_initialGradientImplementation_;
}
void NumericalMathFunctionImplementation::setInitialGradientImplementation(const GradientImplementation & initialGradientImplementation)
{
  p_gradientImplementation_ = initialGradientImplementation;
}

/* Initial hessian implementation accessors */
const NumericalMathFunctionImplementation::HessianImplementation & NumericalMathFunctionImplementation::getInitialHessianImplementation() const
{
  return p_initialHessianImplementation_;
}
void NumericalMathFunctionImplementation::setInitialHessianImplementation(const HessianImplementation & initialHessianImplementation)
{
  p_hessianImplementation_ = initialHessianImplementation;
}

/* Flag for default gradient accessors */
Bool NumericalMathFunctionImplementation::getUseDefaultGradientImplementation() const
{
  return useDefaultGradientImplementation_;
}

void NumericalMathFunctionImplementation::setUseDefaultGradientImplementation(const Bool gradientFlag)
{
  useDefaultGradientImplementation_ = gradientFlag;
}

/* Flag for default hessian accessors */
Bool NumericalMathFunctionImplementation::getUseDefaultHessianImplementation() const
{
  return useDefaultHessianImplementation_;
}

void NumericalMathFunctionImplementation::setUseDefaultHessianImplementation(const Bool hessianFlag)
{
  useDefaultHessianImplementation_ = hessianFlag;
}


/* Gradient according to the marginal parameters */
Matrix NumericalMathFunctionImplementation::parametersGradient(const NumericalPoint & inP) const
{
  return p_evaluationImplementation_->parametersGradient(inP);
}

/* Parameters value and description accessor */
NumericalPointWithDescription NumericalMathFunctionImplementation::getParameters() const
{
  return p_evaluationImplementation_->getParameters();
}

void NumericalMathFunctionImplementation::setParameters(const NumericalPointWithDescription & parameters)
{
  p_evaluationImplementation_->setParameters(parameters);
}


/* Operator () */
NumericalPoint NumericalMathFunctionImplementation::operator() (const NumericalPoint & inP) const
{
  return p_evaluationImplementation_->operator()(inP);
}

/* Operator () */
NumericalSample NumericalMathFunctionImplementation::operator() (const NumericalSample & inSample) const
{
  return p_evaluationImplementation_->operator()(inSample);
}

/* Operator () */
TimeSeries NumericalMathFunctionImplementation::operator() (const TimeSeries & inTimeSeries) const
{
  return p_evaluationImplementation_->operator()(inTimeSeries);
}

/* Method gradient() returns the Jacobian transposed matrix of the function at point */
Matrix NumericalMathFunctionImplementation::gradient(const NumericalPoint & inP) const
{
  if (useDefaultGradientImplementation_) LOGWARN(OSS() << "You are using a default implementation for the gradient. Be careful, your computation can be severely wrong!");
  return p_gradientImplementation_->gradient(inP);
}

/* Method hessian() returns the symetric tensor of the function at point */
SymmetricTensor NumericalMathFunctionImplementation::hessian(const NumericalPoint & inP) const
{
  if (useDefaultHessianImplementation_) LOGWARN(OSS() << "You are using a default implementation for the hessian. Be careful, your computation can be severely wrong!");
  return p_hessianImplementation_->hessian(inP);
}




/* Accessor for input point dimension */
UnsignedLong NumericalMathFunctionImplementation::getInputNumericalPointDimension() const
{
  return p_evaluationImplementation_->getInputDimension();
}

/* Accessor for output point dimension */
UnsignedLong NumericalMathFunctionImplementation::getOutputNumericalPointDimension() const
{
  return p_evaluationImplementation_->getOutputDimension();

}

/* Accessor for input point dimension */
UnsignedLong NumericalMathFunctionImplementation::getInputDimension() const
{
  return p_evaluationImplementation_->getInputDimension();
}

/* Accessor for output point dimension */
UnsignedLong NumericalMathFunctionImplementation::getOutputDimension() const
{
  return p_evaluationImplementation_->getOutputDimension();

}

/* Get the i-th marginal function */
NumericalMathFunctionImplementation::Implementation NumericalMathFunctionImplementation::getMarginal(const UnsignedLong i) const
{
  if (i >= getOutputDimension()) throw InvalidArgumentException(HERE) << "Error: the index of a marginal function must be in the range [0, outputDimension-1]";
  return getMarginal(Indices(1, i));
}

/* Get the function corresponding to indices components */
NumericalMathFunctionImplementation::Implementation NumericalMathFunctionImplementation::getMarginal(const Indices & indices) const
{
  if (!indices.check(getOutputDimension() - 1)) throw InvalidArgumentException(HERE) << "Error: the indices of a marginal function must be in the range [0, outputDimension-1] and  must be different";
  return new NumericalMathFunctionImplementation(p_evaluationImplementation_->getMarginal(indices), p_gradientImplementation_->getMarginal(indices), p_hessianImplementation_->getMarginal(indices));
}

/* Number of calls to the evaluation */
UnsignedLong NumericalMathFunctionImplementation::getEvaluationCallsNumber() const
{
  return p_evaluationImplementation_->getCallsNumber();
}

/* Number of calls to the gradient */
UnsignedLong NumericalMathFunctionImplementation::getGradientCallsNumber() const
{
  return p_gradientImplementation_->getCallsNumber();
}

/* Number of calls to the hessian */
UnsignedLong NumericalMathFunctionImplementation::getHessianCallsNumber() const
{
  return p_hessianImplementation_->getCallsNumber();
}

/* Initialization of the documentation */
void NumericalMathFunctionImplementation::InitializeDocumentation()
{
  if (IsDocumentationInitialized_) return;

  // First, the constants
  ValidConstants_.setName("Valid constants");
  ValidConstants_.add("_e -> Euler's constant (2.71828...)");
  ValidConstants_.add("_pi -> Pi constant (3.14159...)");

  // Second, the functions
  ValidFunctions_.setName("Valid functions");
  ValidFunctions_.add("sin(arg) -> sine function");
  ValidFunctions_.add("cos(arg) -> cosine function");
  ValidFunctions_.add("tan(arg) -> tangent function");
  ValidFunctions_.add("asin(arg) -> inverse sine function");
  ValidFunctions_.add("acos(arg) -> inverse cosine function");
  ValidFunctions_.add("atan(arg) -> inverse tangent function, values in (-pi/2, pi/2)");
  ValidFunctions_.add("atan2(arg1, arg2) -> inverse tangent function, values in (-pi, pi)");
  ValidFunctions_.add("sinh(arg) -> hyperbolic sine function");
  ValidFunctions_.add("cosh(arg) -> hyperbolic cosine function");
  ValidFunctions_.add("tanh(arg) -> hyperbolic tangens function");
  ValidFunctions_.add("asinh(arg) -> inverse hyperbolic sine function");
  ValidFunctions_.add("acosh(arg) -> inverse hyperbolic cosine function");
  ValidFunctions_.add("atanh(arg) -> inverse hyperbolic tangent function");
  ValidFunctions_.add("log2(arg) -> logarithm in base 2");
  ValidFunctions_.add("log10(arg) -> logarithm in base 10");
  ValidFunctions_.add("log(arg) -> logarithm in base e (2.71828...)");
  ValidFunctions_.add("ln(arg) -> alias for log function");
  ValidFunctions_.add("lngamma(arg) -> log of the gamma function");
  ValidFunctions_.add("gamma(arg) -> gamma function");
  ValidFunctions_.add("exp(arg) -> exponential function");
  ValidFunctions_.add("erf(arg) -> error function");
  ValidFunctions_.add("erfc(arg) -> complementary error function");
  ValidFunctions_.add("abs(arg) -> absolute value function");
  ValidFunctions_.add("sqrt(arg) -> square root function");
  ValidFunctions_.add("cbrt(arg) -> cubic root function");
  ValidFunctions_.add("besselJ0(arg) -> 1rst kind Bessel function with parameter 0");
  ValidFunctions_.add("besselJ1(arg) -> 1rst kind Bessel function with parameter 1");
  ValidFunctions_.add("besselY0(arg) -> 2nd kind Bessel function with parameter 0");
  ValidFunctions_.add("besselY1(arg) -> 2nd kind Bessel function with parameter 1");
  ValidFunctions_.add("floor(arg) -> round to nearest integer");
  ValidFunctions_.add("ceil(arg) -> round to nearest integer");
  ValidFunctions_.add("trunc(arg) -> round to nearest integer");
  ValidFunctions_.add("round(arg) -> round to nearest integer");
  ValidFunctions_.add("rint(arg) -> round to nearest integer");
  ValidFunctions_.add("sign(arg) -> sign function -1 if x<0; 1 if x>0");
  ValidFunctions_.add("if(arg1, arg2, arg3) -> if arg1 then arg2 else arg3");
  ValidFunctions_.add("sum(arg1, ..., argn) -> sum of all arguments");
  ValidFunctions_.add("avg(arg1, ..., argn) -> mean value of all arguments");
  ValidFunctions_.add("min(arg1, ..., argn) -> min of all arguments");
  ValidFunctions_.add("max(arg1, ..., argn) -> max of all arguments");

  // Third, the operators
  ValidOperators_.setName("Valid operators");
  ValidOperators_.add("= -> assignement, can only be applied to variable names (priority -1)");
  ValidOperators_.add("and -> logical and (priority 1)");
  ValidOperators_.add("or -> logical or (priority 1)");
  ValidOperators_.add("xor -> logical xor (priority 1)");
  ValidOperators_.add("<= -> less or equal (priority 2)");
  ValidOperators_.add(">= -> greater or equal (priority 2)");
  ValidOperators_.add("!= -> not equal (priority 2)");
  ValidOperators_.add("== -> equal (priority 2)");
  ValidOperators_.add("> -> greater than (priority 2)");
  ValidOperators_.add("< -> less than (priority 2)");
  ValidOperators_.add("+ -> addition (priority 3)");
  ValidOperators_.add("- -> subtraction (priority 3)");
  ValidOperators_.add("* -> multiplication (priority 4)");
  ValidOperators_.add("/ -> division (priority 4)");
  ValidOperators_.add("~ -> logical negation (priority 4)");
  ValidOperators_.add("- -> sign change (priority 4)");
  ValidOperators_.add("^ -> raise x to the power of y (priority 5)");
  IsDocumentationInitialized_ = true;
}

/* Static methods for documentation of analytical fonctions */
Description NumericalMathFunctionImplementation::GetValidConstants()
{
  if (!IsDocumentationInitialized_) InitializeDocumentation();
  return ValidConstants_;
}

Description NumericalMathFunctionImplementation::GetValidFunctions()
{
  if (!IsDocumentationInitialized_) InitializeDocumentation();
  return ValidFunctions_;
}

Description NumericalMathFunctionImplementation::GetValidOperators()
{
  if (!IsDocumentationInitialized_) InitializeDocumentation();
  return ValidOperators_;
}

/* Draw the given 1D marginal output as a function of the given 1D marginal input around the given central point */
Graph NumericalMathFunctionImplementation::draw(const UnsignedLong inputMarginal,
                                                const UnsignedLong outputMarginal,
                                                const NumericalPoint & centralPoint,
                                                const NumericalScalar xMin,
                                                const NumericalScalar xMax,
                                                const UnsignedLong pointNumber) const
{
  return p_evaluationImplementation_->draw(inputMarginal, outputMarginal, centralPoint, xMin, xMax, pointNumber);
}

/* Draw the given 1D marginal output as a function of the given 1D marginal input around the given central point */
Graph NumericalMathFunctionImplementation::draw(const UnsignedLong firstInputMarginal,
                                                const UnsignedLong secondInputMarginal,
                                                const UnsignedLong outputMarginal,
                                                const NumericalPoint & centralPoint,
                                                const NumericalPoint & xMin,
                                                const NumericalPoint & xMax,
                                                const Indices & pointNumber) const
{
  return p_evaluationImplementation_->draw(firstInputMarginal, secondInputMarginal, outputMarginal, centralPoint, xMin, xMax, pointNumber);
}

/* Draw the output of the function with respect to its input when the input and output dimensions are 1 */
Graph NumericalMathFunctionImplementation::draw(const NumericalScalar xMin,
                                                const NumericalScalar xMax,
                                                const UnsignedLong pointNumber) const
{
  return p_evaluationImplementation_->draw(xMin, xMax, pointNumber);
}

/* Draw the output of the function with respect to its input when the input dimension is 2 and the output dimension is 1 */
Graph NumericalMathFunctionImplementation::draw(const NumericalPoint & xMin,
                                                const NumericalPoint & xMax,
                                                const Indices & pointNumber) const
{
  return p_evaluationImplementation_->draw(xMin, xMax, pointNumber);
}

/* Method save() stores the object through the StorageManager */
void NumericalMathFunctionImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "evaluationImplementation_", *p_evaluationImplementation_ );
  adv.saveAttribute( "gradientImplementation_", *p_gradientImplementation_ );
  adv.saveAttribute( "hessianImplementation_", *p_hessianImplementation_ );
  adv.saveAttribute( "initialEvaluationImplementation_", *p_initialEvaluationImplementation_ );
  adv.saveAttribute( "initialGradientImplementation_", *p_initialGradientImplementation_ );
  adv.saveAttribute( "initialHessianImplementation_", *p_initialHessianImplementation_ );
  adv.saveAttribute( "useDefaultGradientImplementation_", useDefaultGradientImplementation_ );
  adv.saveAttribute( "useDefaultHessianImplementation_", useDefaultHessianImplementation_ );
}

/* Method load() reloads the object from the StorageManager */
void NumericalMathFunctionImplementation::load(Advocate & adv)
{
  TypedInterfaceObject<NumericalMathEvaluationImplementation> evaluationValue;
  TypedInterfaceObject<NumericalMathGradientImplementation> gradientValue;
  TypedInterfaceObject<NumericalMathHessianImplementation> hessianValue;
  PersistentObject::load(adv);
  adv.loadAttribute( "evaluationImplementation_", evaluationValue );
  p_evaluationImplementation_ = evaluationValue.getImplementation();
  adv.loadAttribute( "gradientImplementation_", gradientValue );
  p_gradientImplementation_ = gradientValue.getImplementation();
  adv.loadAttribute( "hessianImplementation_", hessianValue );
  p_hessianImplementation_ = hessianValue.getImplementation();
  adv.loadAttribute( "initialEvaluationImplementation_", evaluationValue );
  p_initialEvaluationImplementation_ = evaluationValue.getImplementation();
  adv.loadAttribute( "initialGradientImplementation_", gradientValue );
  p_initialGradientImplementation_ = gradientValue.getImplementation();
  adv.loadAttribute( "initialHessianImplementation_", hessianValue );
  p_initialHessianImplementation_ = hessianValue.getImplementation();
  adv.loadAttribute( "useDefaultGradientImplementation_", useDefaultGradientImplementation_ );
  adv.loadAttribute( "useDefaultHessianImplementation_", useDefaultHessianImplementation_ );
}




END_NAMESPACE_OPENTURNS
