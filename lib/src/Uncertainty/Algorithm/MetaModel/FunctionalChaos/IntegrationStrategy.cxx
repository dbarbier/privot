//                                               -*- C++ -*-
/**
 *  @file  IntegrationStrategy.cxx
 *  @brief An implementation of projection strategy as a leastSquares
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
#include "IntegrationStrategy.hxx"
#include "OSS.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(IntegrationStrategy);


static Factory<IntegrationStrategy> RegisteredFactory("IntegrationStrategy");

/* Default constructor */
IntegrationStrategy::IntegrationStrategy()
  : ProjectionStrategyImplementation()
{
  // Nothing to do
}

/* Parameter constructor */
IntegrationStrategy::IntegrationStrategy(const Distribution & measure)
  : ProjectionStrategyImplementation(measure)
{
  // Nothing to do
}

/* Parameter constructor */
IntegrationStrategy::IntegrationStrategy(const WeightedExperiment & weightedExperiment)
  : ProjectionStrategyImplementation(weightedExperiment)
{
  // Nothing to do
}

/* Parameter constructor */
IntegrationStrategy::IntegrationStrategy(const Distribution & measure,
                                         const WeightedExperiment & weightedExperiment)
  : ProjectionStrategyImplementation(measure, weightedExperiment)
{
  // Nothing to do
}

/* Parameter constructor */
IntegrationStrategy::IntegrationStrategy(const NumericalSample & inputSample,
                                         const NumericalPoint & weights,
                                         const NumericalSample & outputSample)
  : ProjectionStrategyImplementation(inputSample, weights, outputSample)
{
  // Nothing to do
}

/* Parameter constructor */
IntegrationStrategy::IntegrationStrategy(const NumericalSample & inputSample,
                                         const NumericalSample & outputSample)
  : ProjectionStrategyImplementation(inputSample, NumericalPoint(inputSample.getSize(), 1.0 / inputSample.getSize()), outputSample)
{
  // Nothing to do
}

/* Virtual constructor */
IntegrationStrategy * IntegrationStrategy::clone() const
{
  return new IntegrationStrategy(*this);
}

/* String converter */
String IntegrationStrategy::__repr__() const
{
  return OSS() << "class=" << getClassName()
               << " experiment=" << p_weightedExperiment_->__repr__();
}


/* Compute the components alpha_k_p_ by projecting the model on the partial L2 basis
   For the moment, there is no specific strategy for improving the approximation of
   the L2 integral by a finite sum: the same input sample is used for all the calls
   to this method */
void IntegrationStrategy::computeCoefficients(const NumericalMathFunction & function,
                                              const NumericalMathFunctionCollection & partialBasis,
                                              const Indices & indices,
                                              const Indices & addedRanks,
                                              const Indices & conservedRanks,
                                              const Indices & removedRanks,
                                              const UnsignedLong marginalIndex)
{
  // Check if the marginal index is not compatible with the function output dimension
  if (marginalIndex >= function.getOutputDimension()) throw InvalidArgumentException(HERE) << "Error: the marginal index is too large with respect to the function output dimension.";
  // If the input sample has not yet been generated
  // we have to generate it and to evaluate the model
  // upon this sample
  if (inputSample_.getSize() == 0)
    {
      inputSample_  = p_weightedExperiment_->generate(weights_);
      outputSample_ = function(inputSample_);
    }
  // First, copy the coefficients that are common with the previous partial basis
  NumericalPoint alpha(0);
  const UnsignedLong conservedSize(conservedRanks.getSize());
  for (UnsignedLong i = 0; i < conservedSize; ++i)
    alpha.add(alpha_k_p_[conservedRanks[i]]);
  // We have to compute the coefficients associated to the added indices
  const UnsignedLong addedSize(addedRanks.getSize());
  const UnsignedLong sampleSize(inputSample_.getSize());
  for (UnsignedLong i = 0; i < addedSize; ++i)
    {
      const UnsignedLong indexAdded(addedRanks[i]);
      NumericalScalar value(0.0);
      // The integral is approximated by a weighted sum of f * Phi
      for (UnsignedLong j = 0; j < sampleSize; ++j) value += weights_[j] * outputSample_[j][marginalIndex] * partialBasis[indexAdded](inputSample_[j])[0];
      alpha.add(value);
    }
  alpha_k_p_ = alpha;
  // The residual is the mean squared error between the model and the meta model
  residual_p_ = 0.0;
  const UnsignedLong basisDimension(partialBasis.getSize());
  for (UnsignedLong i = 0; i < sampleSize; ++i)
    {
      NumericalScalar value(0.0);
      // Evaluate the meta model on the current integration point
      for (UnsignedLong j = 0; j < basisDimension; ++j) value += alpha_k_p_[j] * partialBasis[j](inputSample_[i])[0];
      // Accumulate the squared residual
      residual_p_ += pow(outputSample_[i][marginalIndex] - value, 2);
    }
  residual_p_ = sqrt(residual_p_) / sampleSize;
  relativeError_p_ = 0.0;
}


/* Method save() stores the object through the StorageManager */
void IntegrationStrategy::save(Advocate & adv) const
{
  ProjectionStrategyImplementation::save(adv);
}


/* Method load() reloads the object from the StorageManager */
void IntegrationStrategy::load(Advocate & adv)
{
  ProjectionStrategyImplementation::load(adv);
}



END_NAMESPACE_OPENTURNS
