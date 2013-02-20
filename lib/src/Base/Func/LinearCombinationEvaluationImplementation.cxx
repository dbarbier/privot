//                                               -*- C++ -*-
/**
 *  @file  LinearCombinationEvaluationImplementation.cxx
 *  @brief The evaluation part of linear combination of polynomials
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
 *  @author dutka
 *  @date   2008-05-21 11:21:38 +0200 (Wed, 21 May 2008)
 */
#include "LinearCombinationEvaluationImplementation.hxx"
#include "OSS.hxx"
#include "PersistentObjectFactory.hxx"
#include "Description.hxx"

BEGIN_NAMESPACE_OPENTURNS



TEMPLATE_CLASSNAMEINIT(PersistentCollection<NumericalMathFunction>);

static Factory<PersistentCollection<NumericalMathFunction> > RegisteredFactory_PC_NMF("PersistentCollection<NumericalMathFunction>");


CLASSNAMEINIT(LinearCombinationEvaluationImplementation);

static Factory<LinearCombinationEvaluationImplementation> RegisteredFactory_LCEI("LinearCombinationEvaluationImplementation");


/* Default constructor */
LinearCombinationEvaluationImplementation::LinearCombinationEvaluationImplementation()
  : NumericalMathEvaluationImplementation(),
    functionsCollection_(0),
    coefficients_(0)
{
  // Nothing to do
}


/* Parameters constructor */
LinearCombinationEvaluationImplementation::LinearCombinationEvaluationImplementation(const NumericalMathFunctionCollection & functionsCollection,
                                                                                     const NumericalPoint & coefficients)
  : NumericalMathEvaluationImplementation(),
    functionsCollection_(0),
    coefficients_(0)
{
  setFunctionsCollectionAndCoefficients(functionsCollection, coefficients);
}


/* Virtual constructor */
LinearCombinationEvaluationImplementation * LinearCombinationEvaluationImplementation::clone() const
{
  return new LinearCombinationEvaluationImplementation(*this);
}


/* String converter */
String LinearCombinationEvaluationImplementation::__repr__() const
{
  return OSS() << "class=" << GetClassName()
               << " functions=" << functionsCollection_
               << " coefficients=" << coefficients_.__repr__();
}

String LinearCombinationEvaluationImplementation::__str__(const String & offset) const
{
  OSS oss(false);
  oss << offset;
  const UnsignedLong size(functionsCollection_.getSize());
  for (UnsignedLong i = 0; i < size; ++i) oss << (i > 0 ? "+" : "") << "(" << coefficients_[i] << ")*" << functionsCollection_[i].getEvaluationImplementation()->__str__();
  return oss;
}


/* Evaluation operator */
NumericalPoint LinearCombinationEvaluationImplementation::operator () (const NumericalPoint & inP) const
{
  const UnsignedLong inputDimension(getInputDimension());
  if (inP.getDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: the given point has an invalid dimension. Expect a dimension " << inputDimension << ", got " << inP.getDimension();
  const UnsignedLong size(functionsCollection_.getSize());
  NumericalPoint result(getOutputDimension());
  for (UnsignedLong i = 0; i < size; ++i) result += coefficients_[i] * functionsCollection_[i](inP);
  ++callsNumber_;
  if (isHistoryEnabled_)
    {
      inputStrategy_.store(inP);
      outputStrategy_.store(result);
    }
  return result;
}

/* Coefficients accessor */
NumericalPoint LinearCombinationEvaluationImplementation::getCoefficients() const
{
  return coefficients_;
}

/* Functions accessor */
LinearCombinationEvaluationImplementation::NumericalMathFunctionCollection LinearCombinationEvaluationImplementation::getFunctionsCollection() const
{
  return functionsCollection_;
}

void LinearCombinationEvaluationImplementation::setFunctionsCollectionAndCoefficients(const NumericalMathFunctionCollection & functionsCollection,
                                                                                      const NumericalPoint & coefficients)
{
  const UnsignedLong size(functionsCollection.getSize());
  // Check for empty functions collection
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a linear combination from an empty collection of functions.";
  // Check for incompatible number of functions and coefficients
  if (size != coefficients.getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot build a linear combination with a different number of functions and coefficients.";
  // Check for coherent input and output dimensions of the functions
  UnsignedLong inputDimension(functionsCollection[0].getInputDimension());
  UnsignedLong outputDimension(functionsCollection[0].getOutputDimension());
  for (UnsignedLong i = 1; i < size; ++i)
    {
      if (functionsCollection[i].getInputDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: the given functions have incompatible input dimension.";
      if (functionsCollection[i].getOutputDimension() != outputDimension) throw InvalidArgumentException(HERE) << "Error: the given functions have incompatible output dimension.";
    }
  functionsCollection_ = functionsCollection;
  coefficients_ = coefficients;
  setDescription(functionsCollection[0].getDescription());
}


/* Input dimension accessor */
UnsignedLong LinearCombinationEvaluationImplementation::getInputDimension() const
{
  return functionsCollection_[0].getInputDimension();
}

/* Output dimension accessor */
UnsignedLong LinearCombinationEvaluationImplementation::getOutputDimension() const
{
  return functionsCollection_[0].getOutputDimension();
}


/* Gradient according to the marginal parameters */
Matrix LinearCombinationEvaluationImplementation::parametersGradient(const NumericalPoint & inP) const
{
  Matrix result(getParameters().getDimension(), getOutputDimension());
  const UnsignedLong size(functionsCollection_.getSize());
  // Get the parameters gradients for each atom and stack them into the result
  UnsignedLong rowIndex(0);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      // Extract the atom gradient
      const Matrix atomParametersGradient(functionsCollection_[i].parametersGradient(inP));
      const UnsignedLong rowDimension(atomParametersGradient.getNbRows());
      const UnsignedLong columnDimension(atomParametersGradient.getNbColumns());
      // Scale the atom gradient and copy it into the result
      const NumericalScalar coefficient(coefficients_[i]);
      for (UnsignedLong j = 0; j < rowDimension; ++j)
        {
          for (UnsignedLong k = 0; k < columnDimension; ++k)
            {
              result(rowIndex, k) = coefficient * atomParametersGradient(j, k);
            }
          ++rowIndex;
        }
    }
  return result;
}

/* Parameters value and description accessor */
NumericalPointWithDescription LinearCombinationEvaluationImplementation::getParameters() const
{
  NumericalPointWithDescription parameters(0);
  Description description(0);
  const UnsignedLong size(functionsCollection_.getSize());
  for (UnsignedLong i = 0; i < size; ++i)
    {
      // Extract the parameters of the current atom
      NumericalPointWithDescription atomParameters(functionsCollection_[i].getParameters());
      Description atomDescription(atomParameters.getDescription());
      const UnsignedLong atomSize(atomParameters.getDimension());
      // Copy the parameters value and description
      for (UnsignedLong j = 0; j < atomSize; ++j)
        {
          parameters.add(atomParameters[i]);
          description.add(atomDescription[i]);
        }
    }
  parameters.setDescription(description);
  return parameters;
}

/* Method save() stores the object through the StorageManager */
void LinearCombinationEvaluationImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "functionsCollection_", functionsCollection_ );
  adv.saveAttribute( "coefficients_", coefficients_ );
}


/* Method load() reloads the object from the StorageManager */
void LinearCombinationEvaluationImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "functionsCollection_", functionsCollection_ );
  adv.loadAttribute( "coefficients_", coefficients_ );
}



END_NAMESPACE_OPENTURNS
