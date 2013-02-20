//                                               -*- C++ -*-
/**
 *  @file  NumericalMathGradientImplementation.cxx
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */
#include "NumericalMathGradientImplementation.hxx"
#include "PersistentObjectFactory.hxx"
#include "AnalyticalNumericalMathEvaluationImplementation.hxx"
#include "ConstantNumericalMathGradientImplementation.hxx"
#include "ComposedNumericalMathGradientImplementation.hxx"


BEGIN_NAMESPACE_OPENTURNS




CLASSNAMEINIT(NumericalMathGradientImplementation);

static Factory<NumericalMathGradientImplementation> RegisteredFactory("NumericalMathGradientImplementation");

/* Default constructor */
NumericalMathGradientImplementation::NumericalMathGradientImplementation()
  : PersistentObject(),
    callsNumber_(0),
    description_()
{
  // Nothing to do
}

/* Virtual constructor */
NumericalMathGradientImplementation * NumericalMathGradientImplementation::clone() const
{
  return new NumericalMathGradientImplementation(*this);
}

/* Comparison operator */
Bool NumericalMathGradientImplementation::operator ==(const NumericalMathGradientImplementation & other) const
{
  return true;
}

/* String converter */
String NumericalMathGradientImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << NumericalMathGradientImplementation::GetClassName()
      << " name=" << getName()
      << " description=" << description_;
  return oss;
}

/* String converter */
String NumericalMathGradientImplementation::__str__(const String & offset) const
{
  return OSS(false) << offset << "NumericalMathGradientImplementation";
}


/* Description Accessor */
void NumericalMathGradientImplementation::setDescription(const Description & description)
{
  description_ = description;
}


/* Description Accessor */
Description NumericalMathGradientImplementation::getDescription() const
{
  return description_;
}

/* Test for actual implementation */
Bool NumericalMathGradientImplementation::isActualImplementation() const
{
  return true;
}

/* Here is the interface that all derived class must implement */

/* Gradient method */
Matrix NumericalMathGradientImplementation::gradient(const NumericalPoint & inP) const
{
  throw NotYetImplementedException(HERE);
}

/* Accessor for input point dimension */
UnsignedLong NumericalMathGradientImplementation::getInputDimension() const
{
  throw NotYetImplementedException(HERE);
}

/* Accessor for output point dimension */
UnsignedLong NumericalMathGradientImplementation::getOutputDimension() const
{
  throw NotYetImplementedException(HERE);
}

/* Get the number of calls to operator() */
UnsignedLong NumericalMathGradientImplementation::getCallsNumber() const
{
  return callsNumber_;
}

/* Get the i-th marginal function */
NumericalMathGradientImplementation::Implementation NumericalMathGradientImplementation::getMarginal(const UnsignedLong i) const
{
  if (i >= getOutputDimension()) throw InvalidArgumentException(HERE) << "Error: the index of a marginal function must be in the range [0, outputDimension-1]";
  return getMarginal(Indices(1, i));
}

/* Get the function corresponding to indices components */
NumericalMathGradientImplementation::Implementation NumericalMathGradientImplementation::getMarginal(const Indices & indices) const
{
  if (!indices.check(getOutputDimension() - 1)) throw InvalidArgumentException(HERE) << "Error: the indices of a marginal function must be in the range [0, outputDimension-1] and  must be different";
  // Here we use the linear algebra representation of the marginal extraction operation in order to extract the marginal gradient.
  // The chain rule gives:
  // D(Af) = AD(f) in our case, instead of D(gof) = Dg(f)Df, so we don't need f as in our case Dg(f) = A is a constant. As we don't
  // have access to f here but only to Df, we build an arbitrary cheap evaluation with the proper dimension in order to reuse the
  // generic implementation of the chain rule for the gradients. We choose to build a null function using an analytical function.
  // Fake f
  const UnsignedLong inputDimension(getInputDimension());
  Description input(inputDimension);
  for (UnsignedLong index = 0; index < inputDimension; ++index)
    input[index] = OSS() << "x" << index;
  const UnsignedLong outputDimension(getOutputDimension());
  Description output(outputDimension);
  for (UnsignedLong index = 0; index < outputDimension; ++index)
    output[index] = OSS() << "y" << index;
  const Description formulas(outputDimension, "0.0");
  const AnalyticalNumericalMathEvaluationImplementation right(input, output, formulas);
  // A
  const UnsignedLong marginalOutputDimension(indices.getSize());
  Matrix gradientExtraction(outputDimension, marginalOutputDimension);
  for (UnsignedLong i = 0; i < marginalOutputDimension; ++i)
    gradientExtraction(indices[i], i) = 1.0;
  const ConstantNumericalMathGradientImplementation leftGradient(gradientExtraction);
  return new ComposedNumericalMathGradientImplementation(leftGradient.clone(), right.clone(), clone());
}

/* Method save() stores the object through the StorageManager */
void NumericalMathGradientImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "callsNumber_", callsNumber_ );
  adv.saveAttribute( "description_", description_ );
}

/* Method load() reloads the object from the StorageManager */
void NumericalMathGradientImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "callsNumber_", callsNumber_ );
  adv.loadAttribute( "description_", description_ );
}

END_NAMESPACE_OPENTURNS
