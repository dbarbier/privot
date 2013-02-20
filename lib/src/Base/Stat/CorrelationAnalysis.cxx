//                                               -*- C++ -*-
/**
 * @file  CorrelationAnalysis.cxx
 * @brief CorrelationAnalysis implements the sensitivity analysis methods based on correlation coefficients
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
#include <cmath>

#include "CorrelationAnalysis.hxx"
#include "Exception.hxx"
#include "LinearModelFactory.hxx"
#include "LinearModel.hxx"
#include "LinearLeastSquares.hxx"

BEGIN_NAMESPACE_OPENTURNS


/* Default constructor */
CorrelationAnalysis::CorrelationAnalysis() {}

/* Compute the Pearson correlation coefficient between the component number index of the input sample and the 1D outputSample */
NumericalScalar CorrelationAnalysis::PearsonCorrelation(const NumericalSample & inputSample,
                                                        const NumericalSample & outputSample,
                                                        const UnsignedLong index)
{
  if (index >= inputSample.getDimension()) throw InvalidArgumentException(HERE) << "Error: given index out of bound";
  if (outputSample.getDimension() != 1) throw InvalidDimensionException(HERE) << "Error: output sample must be 1D";
  if (inputSample.getSize() != outputSample.getSize()) throw InvalidArgumentException(HERE) << "Error: input and output samples must have the same size";
  const UnsignedLong size(inputSample.getSize());
  NumericalSample pairedSample(size, 2);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      pairedSample[i][0] = inputSample[i][index];
      pairedSample[i][1] = outputSample[i][0];
    }
  return pairedSample.computePearsonCorrelation()(0, 1);
}

/* Compute the Spearman correlation coefficient between the component number index of the input sample and the 1D outputSample */
NumericalScalar CorrelationAnalysis::SpearmanCorrelation(const NumericalSample & inputSample,
                                                         const NumericalSample & outputSample,
                                                         const UnsignedLong index)
{
  if (index >= inputSample.getDimension()) throw InvalidArgumentException(HERE) << "Error: given index out of bound";
  if (outputSample.getDimension() != 1) throw InvalidDimensionException(HERE) << "Error: output sample must be 1D";
  if (inputSample.getSize() != outputSample.getSize()) throw InvalidArgumentException(HERE) << "Error: input and output samples must have the same size";
  return PearsonCorrelation(inputSample.rank(), outputSample.rank());
}

/* Compute the Standard Regression Coefficients (SRC) between the input sample and the output sample */
NumericalPoint CorrelationAnalysis::SRC(const NumericalSample & inputSample,
                                        const NumericalSample & outputSample)
{
  if (outputSample.getDimension() != 1) throw InvalidDimensionException(HERE) << "Error: output sample must be 1D";
  if (inputSample.getSize() != outputSample.getSize()) throw InvalidArgumentException(HERE) << "Error: input and output samples must have the same size";
  const UnsignedLong dimension(inputSample.getDimension());
  LinearLeastSquares regressionAlgorithm(inputSample, outputSample);
  regressionAlgorithm.run();
  const NumericalPoint linear(regressionAlgorithm.getLinear() * NumericalPoint(1, 1.0));
  const NumericalScalar varOutput(outputSample.computeVariancePerComponent()[0]);
  NumericalPoint src(inputSample.computeVariancePerComponent());
  for (UnsignedLong i = 0; i < dimension; ++i) src[i] *= linear[i] * linear[i] / varOutput;
  return src;
}

/* Compute the Partial Correlation Coefficients (PCC) between the input sample and the output sample */
NumericalPoint CorrelationAnalysis::PCC(const NumericalSample & inputSample,
                                        const NumericalSample & outputSample)
{
  if (inputSample.getDimension() < 2) throw InvalidDimensionException(HERE) << "Error: input sample must have dimension > 1";
  if (outputSample.getDimension() != 1) throw InvalidDimensionException(HERE) << "Error: output sample must be 1D";
  if (inputSample.getSize() != outputSample.getSize()) throw InvalidArgumentException(HERE) << "Error: input and output samples must have the same size";
  const UnsignedLong dimension(inputSample.getDimension());
  const UnsignedLong size(inputSample.getSize());
  NumericalPoint pcc(dimension);
  // For each component i, perform an analysis on the truncated input sample where Xi has been removed
  NumericalSample truncatedInput(size, dimension - 1);
  NumericalSample remainingInput(size, 1);
  for (UnsignedLong index = 0; index < dimension; ++index)
    {
      // Build the truncated sample
      for (UnsignedLong i = 0; i < size; ++i)
        {
          for (UnsignedLong j = 0; j < index; ++j) truncatedInput[i][j] = inputSample[i][j];
          for (UnsignedLong j = index + 1; j < dimension; ++j) truncatedInput[i][j - 1] = inputSample[i][j];
          remainingInput[i][0] = inputSample[i][index];
        }
      // Build the linear models
      const LinearModel outputVersusTruncatedInput(LinearModelFactory().build(truncatedInput, outputSample));
      const LinearModel remainingVersusTruncatedInput(LinearModelFactory().build(truncatedInput, remainingInput));
      // Compute the correlation between the residuals
      const NumericalSample residualOutput(outputVersusTruncatedInput.getResidual(truncatedInput, outputSample));
      const NumericalSample residualRemaining(remainingVersusTruncatedInput.getResidual(truncatedInput, remainingInput));
      pcc[index] = PearsonCorrelation(residualOutput, residualRemaining);
    }
  return pcc;
}

/* Compute the Standard Rank Regression Coefficients (SRRC) between the input sample and the output sample */
NumericalPoint CorrelationAnalysis::SRRC(const NumericalSample & inputSample,
                                         const NumericalSample & outputSample)
{
  if (outputSample.getDimension() != 1) throw InvalidDimensionException(HERE) << "Error: output sample must be 1D";
  if (inputSample.getSize() != outputSample.getSize()) throw InvalidArgumentException(HERE) << "Error: input and output samples must have the same size";
  return SRC(inputSample.rank(), outputSample.rank());
}

/* Compute the Partial Rank Correlation Coefficients (PRCC) between the input sample and the output sample */
NumericalPoint CorrelationAnalysis::PRCC(const NumericalSample & inputSample,
                                         const NumericalSample & outputSample)
{
  // Perform the basic checks of the inputs, to avoid costly ranking if finally PCC will fail
  if (inputSample.getDimension() < 2) throw InvalidDimensionException(HERE) << "Error: input sample must have dimension > 1";
  if (outputSample.getDimension() != 1) throw InvalidDimensionException(HERE) << "Error: output sample must be 1D";
  if (inputSample.getSize() != outputSample.getSize()) throw InvalidArgumentException(HERE) << "Error: input and output samples must have the same size";
  return PCC(inputSample.rank(), outputSample.rank());
}

END_NAMESPACE_OPENTURNS
