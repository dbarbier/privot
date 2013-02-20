//                                               -*- C++ -*-
/**
 *  @file  AggregatedEvaluationImplementation.cxx
 *  @brief The evaluation part of an aggregation of functions from R^n to R^p_1,...,R^n to R^p_k
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
#include "AggregatedNumericalMathEvaluationImplementation.hxx"
#include "OSS.hxx"
#include "PersistentObjectFactory.hxx"
#include "Description.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(AggregatedNumericalMathEvaluationImplementation);

static Factory<AggregatedNumericalMathEvaluationImplementation> RegisteredFactory("AggregatedNumericalMathEvaluationImplementation");


/* Default constructor */
AggregatedNumericalMathEvaluationImplementation::AggregatedNumericalMathEvaluationImplementation()
  : NumericalMathEvaluationImplementation(),
    functionsCollection_(0),
    outputDimension_(0)
{
  // Nothing to do
}


/* Parameters constructor */
AggregatedNumericalMathEvaluationImplementation::AggregatedNumericalMathEvaluationImplementation(const NumericalMathFunctionCollection & functionsCollection)
  : NumericalMathEvaluationImplementation(),
    functionsCollection_(0),
    outputDimension_(0)
{
  setFunctionsCollection(functionsCollection);
}


/* Virtual constructor */
AggregatedNumericalMathEvaluationImplementation * AggregatedNumericalMathEvaluationImplementation::clone() const
{
  return new AggregatedNumericalMathEvaluationImplementation(*this);
}


/* String converter */
String AggregatedNumericalMathEvaluationImplementation::__repr__() const
{
  return OSS() << "class=" << GetClassName()
               << " functions=" << functionsCollection_;
}

String AggregatedNumericalMathEvaluationImplementation::__str__(const String & offset) const
{
  return OSS() << functionsCollection_.__str__(offset);
}


/* Evaluation operator */
NumericalPoint AggregatedNumericalMathEvaluationImplementation::operator () (const NumericalPoint & inP) const
{
  const UnsignedLong inputDimension(getInputDimension());
  if (inP.getDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: the given point has an invalid dimension. Expect a dimension " << inputDimension << ", got " << inP.getDimension();
  ++callsNumber_;
  const UnsignedLong size(functionsCollection_.getSize());
  NumericalPoint result(outputDimension_);
  UnsignedLong outputIndex(0);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalPoint atomValue(functionsCollection_[i](inP));
      const UnsignedLong atomDimension(atomValue.getDimension());
      for (UnsignedLong j = 0; j < atomDimension; ++j)
        {
          result[outputIndex] = atomValue[j];
          ++outputIndex;
        }

    }
  if (isHistoryEnabled_)
    {
      inputStrategy_.store(inP);
      outputStrategy_.store(result);
    }
  return result;
}


NumericalSample AggregatedNumericalMathEvaluationImplementation::operator () (const NumericalSample & inS) const
{
  const UnsignedLong inputDimension = getInputDimension();
  if (inS.getDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: the given point has an invalid dimension. Expect a dimension " << inputDimension << ", got " << inS.getDimension();
  ++ callsNumber_;
  const UnsignedLong collectionSize = functionsCollection_.getSize();
  const UnsignedLong size = inS.getSize();
  NumericalSample result(size, outputDimension_);
  UnsignedLong outputIndex = 0;
  for ( UnsignedLong k = 0; k < collectionSize; ++ k )
    {
      const NumericalSample atomValue(functionsCollection_[k](inS));
      const UnsignedLong atomDimension = atomValue.getDimension();
      for ( UnsignedLong i = 0; i < size; ++ i )
        for ( UnsignedLong j = 0; j < atomDimension; ++ j )
          result[i][outputIndex + j] = atomValue[i][j];
      outputIndex += atomDimension;
    }
  result.setDescription(getOutputDescription());
  if (isHistoryEnabled_)
    {
      inputStrategy_.store(inS);
      outputStrategy_.store(result);
    }
  return result;
}


/* Functions accessor */
AggregatedNumericalMathEvaluationImplementation::NumericalMathFunctionCollection AggregatedNumericalMathEvaluationImplementation::getFunctionsCollection() const
{
  return functionsCollection_;
}

void AggregatedNumericalMathEvaluationImplementation::setFunctionsCollection(const NumericalMathFunctionCollection & functionsCollection)
{
  const UnsignedLong size(functionsCollection.getSize());
  // Check for empty functions collection
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build an aggregated function from an empty collection of functions.";
  // Check for coherent input and output dimensions of the functions
  UnsignedLong inputDimension(functionsCollection[0].getInputDimension());
  outputDimension_ = functionsCollection[0].getOutputDimension();
  Description description(functionsCollection[0].getDescription());
  if (outputDimension_ == 0) throw InvalidArgumentException(HERE) << "Error: cannot build an aggregated function with atoms of null output dimension.";
  for (UnsignedLong i = 1; i < size; ++i)
    {
      if (functionsCollection[i].getInputDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: the given functions have incompatible input dimension.";
      const UnsignedLong atomOutputDimension(functionsCollection[i].getOutputDimension());
      if (atomOutputDimension == 0) throw InvalidArgumentException(HERE) << "Error: cannot build an aggregated function with atoms of null output dimension.";
      outputDimension_ += atomOutputDimension;
      const Description outputDescription(functionsCollection[i].getOutputDescription());
      for (UnsignedLong j = 0; j < atomOutputDimension; ++j)
        description.add(outputDescription[j]);
    }
  functionsCollection_ = functionsCollection;
  setDescription(description);
}


/* Input dimension accessor */
UnsignedLong AggregatedNumericalMathEvaluationImplementation::getInputDimension() const
{
  return functionsCollection_[0].getInputDimension();
}

/* Output dimension accessor */
UnsignedLong AggregatedNumericalMathEvaluationImplementation::getOutputDimension() const
{
  return outputDimension_;
}


/* Gradient according to the marginal parameters */
Matrix AggregatedNumericalMathEvaluationImplementation::parametersGradient(const NumericalPoint & inP) const
{
  throw NotYetImplementedException(HERE);
}

/* Parameters value and description accessor */
NumericalPointWithDescription AggregatedNumericalMathEvaluationImplementation::getParameters() const
{
  throw NotYetImplementedException(HERE);
}

/* Method save() stores the object through the StorageManager */
void AggregatedNumericalMathEvaluationImplementation::save(Advocate & adv) const
{
  NumericalMathEvaluationImplementation::save(adv);
  adv.saveAttribute( "functionsCollection_", functionsCollection_ );
  adv.saveAttribute( "outputDimension_", outputDimension_ );
}


/* Method load() reloads the object from the StorageManager */
void AggregatedNumericalMathEvaluationImplementation::load(Advocate & adv)
{
  NumericalMathEvaluationImplementation::load(adv);
  adv.loadAttribute( "functionsCollection_", functionsCollection_ );
  adv.loadAttribute( "outputDimension_", outputDimension_ );
}


END_NAMESPACE_OPENTURNS
