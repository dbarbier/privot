//                                               -*- C++ -*-
/**
 * @file  SensitivityAnalysis.cxx
 * @brief SensitivityAnalysis implements the sensitivity analysis methods based on Sobol coefficients
 *
 * Copyright (C) 2005-2013 EDF-EADS-Phimeca
 *
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 *
 *
 * @author
 * @date
 */
#include <cstdlib>
#include <fstream>

#include "SensitivityAnalysis.hxx"
#include "Path.hxx"
#include "ResourceMap.hxx"
#include "Log.hxx"
#include "Exception.hxx"
#include "Os.hxx"
#include "OTconfig.hxx"
#include "Pie.hxx"
#include "Description.hxx"

BEGIN_NAMESPACE_OPENTURNS


/* Constructor with input samples and model */
SensitivityAnalysis::SensitivityAnalysis(const NumericalSample & inputSample1,
                                         const NumericalSample & inputSample2,
                                         const NumericalMathFunction & model):
  inputSample1_(inputSample1),
  inputSample2_(inputSample2),
  model_(model),
  alreadyComputedOrder_(0)
{
  if (inputSample1_.getDimension() != inputSample2_.getDimension()) throw InvalidDimensionException(HERE) << "Error: input samples must have the same dimension";
  if (inputSample1_.getSize() != inputSample2_.getSize()) throw InvalidArgumentException(HERE) << "Error: input samples must have the same size";
  if (inputSample1_.getDimension() != model_.getInputDimension()) throw InvalidDimensionException(HERE) << "Error: input samples must have the same dimension as the model input dimension";

  // initialize blockSize
  const UnsignedLong size( inputSample1_.getSize() );
  blockSize_ = std::min( size, ResourceMap::GetAsUnsignedLong( "SensitivityAnalysis-DefaultBlockSize" ) );
}

/* Compute all the Sobol indices */
void SensitivityAnalysis::computeSobolIndices(const UnsignedLong order) const
{
  const UnsignedLong inputDimension( inputSample1_.getDimension() );
  const UnsignedLong outputDimension( model_.getOutputDimension() );
  const UnsignedLong size( inputSample1_.getSize() );

  // allocate indices
  firstOrderIndice_ = NumericalSample( outputDimension, inputDimension );
  totalOrderIndice_ = NumericalSample( outputDimension, inputDimension );
  if (order >= 2)
    {
      secondOrderIndice_ = SymmetricTensor( inputDimension, outputDimension );
    }

  // this avoids to store huge input samples while allowing for multi-threading
  const UnsignedLong maximumOuterSampling( static_cast<UnsignedLong>( ceil( 1.0 * size / blockSize_ ) ) );
  const UnsignedLong modulo ( size % blockSize_ );
  const UnsignedLong lastBlockSize( modulo == 0 ? blockSize_ : modulo );

  // 2*N evaluations
  const NumericalSample outputSample1( model_(inputSample1_) );
  const NumericalSample outputSample2( model_(inputSample2_) );

  NumericalPoint sample1SquareMean( outputSample1.computeMean() );// mean over one sample, used for total order indices
  for ( UnsignedLong j = 0; j < outputDimension; ++ j )
    {
      sample1SquareMean[j] *=  sample1SquareMean[j];
    }
  NumericalPoint crossSquareMean( outputDimension, 0.0 );// cross-mean over both samples, used for 1st order indices
  for ( UnsignedLong i = 0; i < size; ++ i)
    {
      for ( UnsignedLong j = 0; j < outputDimension; ++ j )
        {
          crossSquareMean[j] += outputSample1[i][j] * outputSample2[i][j] / size;
        }
    }
  const NumericalPoint sample1Variance( outputSample1.computeVariancePerComponent() );

  // for each block ...
  for ( UnsignedLong outerSampling = 0; outerSampling < maximumOuterSampling; ++ outerSampling )
    {
      // the last block can be smaller
      const UnsignedLong effectiveBlockSize( outerSampling < ( maximumOuterSampling - 1 ) ? blockSize_ : lastBlockSize );

      // k*N evaluations
      NumericalSample scrambledAllOutputBlock1( 0, outputDimension );

      // k*N evaluations ( only 2nd order )
      NumericalSample scrambledAllOutputBlock2( 0, outputDimension );

      for ( UnsignedLong k1 = 0; k1 < inputDimension; ++ k1 )
        {
          NumericalSample scrambledInputBlock1( effectiveBlockSize, inputDimension );
          for ( UnsignedLong blockIndex = 0; blockIndex < effectiveBlockSize; ++ blockIndex )
            {
              scrambledInputBlock1[blockIndex] = inputSample1_[outerSampling * blockSize_ + blockIndex];
              scrambledInputBlock1[blockIndex][k1] = inputSample2_[outerSampling * blockSize_ + blockIndex][k1];
            }
          const NumericalSample scrambledOutputBlock1( model_( scrambledInputBlock1 ) );
          scrambledAllOutputBlock1.add( scrambledOutputBlock1 );

          if ( order >= 2 )
            {
              NumericalSample scrambledInputBlock2( effectiveBlockSize , inputDimension );
              for ( UnsignedLong blockIndex = 0; blockIndex < effectiveBlockSize; ++ blockIndex )
                {
                  scrambledInputBlock2[blockIndex] = inputSample2_[outerSampling * blockSize_ + blockIndex];
                  scrambledInputBlock2[blockIndex][k1] = inputSample1_[outerSampling * blockSize_ + blockIndex][k1];
                }
              const NumericalSample scrambledOutputBlock2( model_( scrambledInputBlock2 ) );
              scrambledAllOutputBlock2.add( scrambledOutputBlock2 );
            }

          for ( UnsignedLong j = 0; j < outputDimension; ++ j )
            {
              for ( UnsignedLong blockIndex = 0; blockIndex < effectiveBlockSize; ++ blockIndex )
                {
                  // this is the core idea of the Saltelli 2002 algorithm
                  firstOrderIndice_[j][k1] += scrambledOutputBlock1[blockIndex][j] * outputSample2[outerSampling * blockSize_ + blockIndex][j] / ( size - 1 );
                  totalOrderIndice_[j][k1] += scrambledOutputBlock1[blockIndex][j] * outputSample1[outerSampling * blockSize_ + blockIndex][j] / ( size - 1 );
                  if ( order >= 2 )
                    {
                      for ( UnsignedLong k2 = 0; k2 < k1; ++ k2 )
                        {
                          // here we could also get the closed second order effects with the same trick as above without any further computation
                          secondOrderIndice_(k1, k2, j) += scrambledAllOutputBlock1[k1 * effectiveBlockSize + blockIndex][j] * scrambledAllOutputBlock2[k2 * effectiveBlockSize + blockIndex][j] / (size - 1);
                        }
                    }
                }
            }
        }

    }

  // normalize indices
  for ( UnsignedLong j = 0; j < outputDimension; ++ j )
    {
      for ( UnsignedLong k = 0; k < inputDimension; ++ k )
        {
          firstOrderIndice_[j][k] = (firstOrderIndice_[j][k] - crossSquareMean[j]) / sample1Variance[j];
          if ((firstOrderIndice_[j][k] < 0.0) || (firstOrderIndice_[j][k] > 1.0)) LOGWARN(OSS() << "The estimated first order total Sobol index (" << k << ") is not in the range [0, 1]. You may increase the sampling size.");

          totalOrderIndice_[j][k] = 1.0 - (totalOrderIndice_[j][k] - sample1SquareMean[j]) / sample1Variance[j];
          if ((totalOrderIndice_[j][k] < 0.0) || (totalOrderIndice_[j][k] > 1.0)) LOGWARN(OSS() << "The estimated total order total Sobol index (" << k << ") is not in the range [0, 1]. You may increase the sampling size.");

        }
      if (order >= 2)
        {
          for ( UnsignedLong k1 = 0; k1 < inputDimension; ++ k1 )
            {
              for ( UnsignedLong k2 = 0; k2 < k1; ++ k2 )
                {
                  secondOrderIndice_(k1, k2, j) = (secondOrderIndice_(k1, k2, j) - crossSquareMean[j]) / sample1Variance[j] - firstOrderIndice_[j][k1] - firstOrderIndice_[j][k2];
                  if ((secondOrderIndice_(k1, k2, j) < 0.0) || (secondOrderIndice_(k1, k2, j) > 1.0)) LOGWARN(OSS() << "The estimated second order Sobol index (" << k1 << ", " << k2 << ") is not in the range [0, 1]. You may increase the sampling size.");
                }
            }
        }
    }

  alreadyComputedOrder_ = order;
}


/* First Order indices accessor */
NumericalPoint SensitivityAnalysis::getFirstOrderIndices(const UnsignedLong marginalIndex) const
{
  if (alreadyComputedOrder_ < 1) computeSobolIndices( 1 );
  return firstOrderIndice_[marginalIndex];
}

/* Second order indices accessor */
SymmetricMatrix SensitivityAnalysis::getSecondOrderIndices(const UnsignedLong marginalIndex) const
{
  if (alreadyComputedOrder_ < 2) computeSobolIndices( 2 );
  return secondOrderIndice_.getSheet( marginalIndex );
}

/* Total Order indices accessor */
NumericalPoint SensitivityAnalysis::getTotalOrderIndices(const UnsignedLong marginalIndex) const
{
  if (alreadyComputedOrder_ < 1) computeSobolIndices( 1 );
  return totalOrderIndice_[marginalIndex];
}

/* Block size accessor */
void SensitivityAnalysis::setBlockSize(const UnsignedLong blockSize)
{
  blockSize_ = blockSize;
}

UnsignedLong SensitivityAnalysis::getBlockSize() const
{
  return blockSize_;
}

/* ImportanceFactors graph */
Graph SensitivityAnalysis::DrawImportanceFactors(const NumericalPointWithDescription & importanceFactors,
                                                 const String & title)
{
  return DrawImportanceFactors(importanceFactors, importanceFactors.getDescription(), title);
}

/* ImportanceFactors graph */
Graph SensitivityAnalysis::DrawImportanceFactors(const NumericalPoint & values,
                                                 const Description & names,
                                                 const String & title)
{
  /* build data for the pie */
  const UnsignedLong dimension(values.getDimension());
  if (dimension == 0) throw InvalidArgumentException(HERE) << "Error: cannot draw an importance factors pie based on empty data.";
  if ((names.getSize() != 0) && (names.getSize() != dimension)) throw InvalidArgumentException(HERE) << "Error: the names size must match the value dimension.";
  NumericalScalar l1Norm(0.0);
  for (UnsignedLong i = 0; i < dimension; ++i) l1Norm += fabs(values[i]);
  if (l1Norm == 0.0) throw InvalidArgumentException(HERE) << "Error: cannot draw an importance factors pie based on null data.";
  NumericalPoint data(dimension);
  /* Normalization */
  for (UnsignedLong i = 0; i < dimension; ++i) data[i] = values[i] / l1Norm;
  /* we build the pie */
  Pie importanceFactorsPie(data);

  /* build labels and colors for the pie */
  Description palette(dimension);
  Description labels(dimension);
  Description description(names);
  // If no description has been given for the input distribution components, give standard ones
  if (description.getSize() != dimension)
    {
      description = Description(dimension);
      for (UnsignedLong i = 0; i < dimension; ++i) description[i] = String(OSS() << "Component " << i);
    }
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      OSS oss(false);
      oss << description[i] << " : " << std::fixed;
      oss.setPrecision(1);
      oss << 100.0 * data[i] << "%";
      labels[i] = oss;
    }
  /* we complete the pie */
  importanceFactorsPie.setLabels(labels);
  importanceFactorsPie.buildDefaultPalette();
  /* we build the graph with a title */
  Graph importanceFactorsGraph(title);
  /* we embed the pie into the graph */
  importanceFactorsGraph.add(importanceFactorsPie);
  return importanceFactorsGraph;
}

END_NAMESPACE_OPENTURNS


