//                                               -*- C++ -*-
/**
 *  @file  DualLinearCombinationEvaluationImplementation.cxx
 *  @brief The evaluation part of functional linear combination of vectors
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
#include "DualLinearCombinationEvaluationImplementation.hxx"
#include "OSS.hxx"
#include "PersistentObjectFactory.hxx"
#include "Description.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(DualLinearCombinationEvaluationImplementation);

static Factory<DualLinearCombinationEvaluationImplementation> RegisteredFactory("DualLinearCombinationEvaluationImplementation");


/* Default constructor */
DualLinearCombinationEvaluationImplementation::DualLinearCombinationEvaluationImplementation()
  : NumericalMathEvaluationImplementation(),
    functionsCollection_(0),
    coefficients_(0, 0)
{
  // Nothing to do
}


/* Parameters constructor */
DualLinearCombinationEvaluationImplementation::DualLinearCombinationEvaluationImplementation(const NumericalMathFunctionCollection & functionsCollection,
                                                                                             const NumericalSample & coefficients)
  : NumericalMathEvaluationImplementation(),
    functionsCollection_(0),
    coefficients_(0, 0)
{
  setFunctionsCollectionAndCoefficients(functionsCollection, coefficients);
}


/* Virtual constructor */
DualLinearCombinationEvaluationImplementation * DualLinearCombinationEvaluationImplementation::clone() const
{
  return new DualLinearCombinationEvaluationImplementation(*this);
}

/* Description accessor */
void DualLinearCombinationEvaluationImplementation::setDescription(const Description & description)
{
  NumericalMathEvaluationImplementation::setDescription(description);
  const Description inputDescription(getInputDescription());
  const UnsignedLong inputDimension(getInputDimension());
  const UnsignedLong size(functionsCollection_.getSize());
  for (UnsignedLong i = 0; i < size; ++i)
    {
      Description atomDescription(functionsCollection_[i].getDescription());
      for (UnsignedLong j = 0; j < inputDimension; ++j) atomDescription[j] = inputDescription[j];
      functionsCollection_[i].setDescription(atomDescription);
    }
}

/* String converter */
String DualLinearCombinationEvaluationImplementation::__repr__() const
{
  return OSS() << "class=" << GetClassName()
               << " functions=" << functionsCollection_
               << " coefficients=" << coefficients_.__repr__();
}

String DualLinearCombinationEvaluationImplementation::__str__(const String & offset) const
{
  OSS oss(false);
  oss << offset;
  const UnsignedLong size(functionsCollection_.getSize());
  const UnsignedLong outputDimension(getOutputDimension());
  Bool first(true);
  static const String valid("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_()[]{}^*/");
  for (UnsignedLong i = 0; i < size; ++i)
    {
      if (outputDimension == 1)
        {
          const NumericalScalar value(coefficients_[i][0]);
          if (value != 0.0)
            {
              if (first) oss << value;
              else if (value > 0.0) oss << " + " << value;
              else oss << " - " << -value;
              first = false;
              const String expr(functionsCollection_[i].getEvaluationImplementation()->__str__());
              // Print the function factor only if it is different from 1
              if (expr != "1")
                {
                  oss << " * ";

                  const Bool complexString(expr.find_first_not_of(valid) != String::npos);
                  if (complexString) oss << "(";
                  oss << expr;
                  if (complexString) oss << ")";
                }
            }
        } // output dimension == 1
      else
        {
          if (first) oss << coefficients_[i];
          else oss << " + " << coefficients_[i];
          first = false;
          const String expr(functionsCollection_[i].getEvaluationImplementation()->__str__());
          if (expr != "1")
            {
              oss << " * ";

              const Bool complexString(expr.find_first_not_of(valid) != String::npos);
              if (complexString) oss << "(";
              oss << expr;
              if (complexString) oss << ")";
            }
        }
    }
  return oss;
}


/* Evaluation operator */
NumericalPoint DualLinearCombinationEvaluationImplementation::operator () (const NumericalPoint & inP) const
{
  const UnsignedLong inputDimension(getInputDimension());
  if (inP.getDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: the given point has an invalid dimension. Expect a dimension " << inputDimension << ", got " << inP.getDimension();
  const UnsignedLong size(functionsCollection_.getSize());
  NumericalPoint result(getOutputDimension());
  for (UnsignedLong i = 0; i < size; ++i) result += coefficients_[i] * functionsCollection_[i](inP)[0];
  ++callsNumber_;
  if (isHistoryEnabled_)
    {
      inputStrategy_.store(inP);
      outputStrategy_.store(result);
    }
  return result;
}

/* Coefficients accessor */
NumericalSample DualLinearCombinationEvaluationImplementation::getCoefficients() const
{
  return coefficients_;
}

/* Functions accessor */
DualLinearCombinationEvaluationImplementation::NumericalMathFunctionCollection DualLinearCombinationEvaluationImplementation::getFunctionsCollection() const
{
  return functionsCollection_;
}

void DualLinearCombinationEvaluationImplementation::setFunctionsCollectionAndCoefficients(const NumericalMathFunctionCollection & functionsCollection,
                                                                                          const NumericalSample & coefficients)
{
  const UnsignedLong size(functionsCollection.getSize());
  // Check for empty functions collection
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a linear combination from an empty collection of functions.";
  // Check for incompatible number of functions and coefficients
  if (size != coefficients.getSize()) throw InvalidArgumentException(HERE) << "Error: cannot build a linear combination with a different number of functions and coefficients.";
  // Check for coherent input and output dimensions of the functions
  UnsignedLong inputDimension(functionsCollection[0].getInputDimension());
  for (UnsignedLong i = 1; i < size; ++i)
    {
      if (functionsCollection[i].getInputDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: the given functions have incompatible input dimension.";
      if (functionsCollection[i].getOutputDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given functions must have a one dimensional output.";
    }
  functionsCollection_ = functionsCollection;
  coefficients_ = coefficients;
  Description description(0);
  Description inputDescription(functionsCollection[0].getInputDescription());
  for (UnsignedLong i = 0; i < inputDescription.getSize(); ++i)
    {
      if (inputDescription[i].size() == 0) description.add(OSS() << "x" << i);
      else description.add(inputDescription[i]);
    }
  Description outputDescription(coefficients.getDescription());
  for (UnsignedLong i = 0; i < outputDescription.getSize(); ++i)
    {
      if (outputDescription[i].size() == 0) description.add(OSS() << "y" << i);
      else description.add(outputDescription[i]);
    }
  setDescription(description);
}


/* Input dimension accessor */
UnsignedLong DualLinearCombinationEvaluationImplementation::getInputDimension() const
{
  return functionsCollection_[0].getInputDimension();
}

/* Output dimension accessor */
UnsignedLong DualLinearCombinationEvaluationImplementation::getOutputDimension() const
{
  return coefficients_.getDimension();
}


/* Gradient according to the marginal parameters */
Matrix DualLinearCombinationEvaluationImplementation::parametersGradient(const NumericalPoint & inP) const
{
  Matrix result(getParameters().getDimension(), getOutputDimension());
  // const UnsignedLong size(functionsCollection_.getSize());
  // // Get the parameters gradients for each atom and stack them into the result
  // UnsignedLong rowIndex(0);
  // for (UnsignedLong i = 0; i < size; ++i)
  //   {
  //     // Extract the atom gradient
  //     const Matrix atomParametersGradient(functionsCollection_[i].parametersGradient(inP));
  //     const UnsignedLong rowDimension(atomParametersGradient.getNbRows());
  //     const UnsignedLong columnDimension(atomParametersGradient.getNbColumns());
  //     // Scale the atom gradient and copy it into the result
  //     const NumericalScalar coefficient(coefficients_[i]);
  //     for (UnsignedLong j = 0; j < rowDimension; ++j)
  //       {
  //         for (UnsignedLong k = 0; k < columnDimension; ++k)
  //           {
  //             result(rowIndex, k) = coefficient * atomParametersGradient(j, k);
  //           }
  //         ++rowIndex;
  //       }
  //   }
  return result;
}

/* Parameters value and description accessor */
NumericalPointWithDescription DualLinearCombinationEvaluationImplementation::getParameters() const
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
void DualLinearCombinationEvaluationImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "functionsCollection_", functionsCollection_ );
  adv.saveAttribute( "coefficients_", coefficients_ );
}


/* Method load() reloads the object from the StorageManager */
void DualLinearCombinationEvaluationImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "functionsCollection_", functionsCollection_ );
  adv.loadAttribute( "coefficients_", coefficients_ );
}



END_NAMESPACE_OPENTURNS
