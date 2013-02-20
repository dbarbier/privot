//                                               -*- C++ -*-
/**
 * @file  CorrelationAnalysis.hxx
 * @brief CorrelationAnalysis implements computation of correlation coefficients
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
 *  Id      CorrelationAnalysis.hxx 2392 2012-02-17 18:35:43Z schueller
 */

#ifndef OPENTURNS_CORRELATIONANALYSIS_HXX
#define OPENTURNS_CORRELATIONANALYSIS_HXX

#include "OTprivate.hxx"
#include "NumericalSample.hxx"
#include "NumericalPoint.hxx"
#include "NumericalMathFunction.hxx"
#include "SymmetricTensor.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class CorrelationAnalysis
 *
 */

class CorrelationAnalysis
{
public:


  /** Default constructor */
  CorrelationAnalysis();

  /** Compute the Pearson correlation coefficient between the component number index of the input sample and the 1D outputSample */
  static NumericalScalar PearsonCorrelation(const NumericalSample & inputSample,
                                            const NumericalSample & outputSample,
                                            const UnsignedLong index = 0);

  /** Compute the Spearman correlation coefficient between the component number index of the input sample and the 1D outputSample */
  static NumericalScalar SpearmanCorrelation(const NumericalSample & inputSample,
                                             const NumericalSample & outputSample,
                                             const UnsignedLong index = 0);

  /** Compute the Standard Regression Coefficients (SRC) between the input sample and the output sample */
  static NumericalPoint SRC(const NumericalSample & inputSample,
                            const NumericalSample & outputSample);

  /** Compute the Partial Correlation Coefficients (PCC) between the input sample and the output sample */
  static NumericalPoint PCC(const NumericalSample & inputSample,
                            const NumericalSample & outputSample);

  /** Compute the Standard Rank Regression Coefficients (SRRC) between the input sample and the output sample */
  static NumericalPoint SRRC(const NumericalSample & inputSample,
                             const NumericalSample & outputSample);

  /** Compute the Partial Rank Correlation Coefficients (PRCC) between the input sample and the output sample */
  static NumericalPoint PRCC(const NumericalSample & inputSample,
                             const NumericalSample & outputSample);

}; /* class CorrelationAnalysis */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_CORRELATIONANALYSIS_HXX */
