//                                               -*- C++ -*-
/**
 * @file  SensitivityAnalysis.hxx
 * @brief SensitivityAnalysis implements the sensitivity analysis methods based on Sobol coefficients
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
 * @author
 * @date
 */

#ifndef OPENTURNS_SENSITIVITYANALYSIS_HXX
#define OPENTURNS_SENSITIVITYANALYSIS_HXX

#include "OTprivate.hxx"
#include "NumericalSample.hxx"
#include "NumericalMathFunction.hxx"
#include "NumericalPoint.hxx"
#include "Description.hxx"
#include "NumericalPointWithDescription.hxx"
#include "Graph.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class SensitivityAnalysis
 *
 */

class SensitivityAnalysis
{
public:

  /** Constructor with input samples and model */
  SensitivityAnalysis(const NumericalSample & inputSample1,
                      const NumericalSample & inputSample2,
                      const NumericalMathFunction & model);

  /** First order indices accessor */
  NumericalPoint getFirstOrderIndices(const UnsignedLong marginalIndex = 0) const;

  /** Second order indices accessor */
  SymmetricMatrix getSecondOrderIndices(const UnsignedLong marginalIndex = 0) const;

  /** Total order indices accessor */
  NumericalPoint getTotalOrderIndices(UnsignedLong marginalIndex = 0) const;

  /** Block size accessor */
  void setBlockSize(const UnsignedLong blockSize);
  UnsignedLong getBlockSize() const;

  /** ImportanceFactors graph */
  static Graph DrawImportanceFactors(const NumericalPointWithDescription & importanceFactors,
                                     const String & title = "Importance Factors");

  /** ImportanceFactors graph */
  static Graph DrawImportanceFactors(const NumericalPoint & values,
                                     const Description & names,
                                     const String & title = "Importance Factors");

private:
  /** Compute all the Sobol indices */
  void computeSobolIndices(const UnsignedLong order) const;

  /** Two independently generated samples */
  NumericalSample inputSample1_;
  NumericalSample inputSample2_;

  /** The model */
  NumericalMathFunction model_;

  /** First order indices */
  mutable NumericalSample firstOrderIndice_;

  /** Total order indices */
  mutable NumericalSample totalOrderIndice_;

  /** Second order indices */
  mutable SymmetricTensor secondOrderIndice_;

  /** Size of blocks */
  UnsignedLong blockSize_;

  /** Store the maximum order already computed */
  mutable UnsignedLong alreadyComputedOrder_;

}; /* class SensitivityAnalysis */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SENSITIVITYANALYSIS_HXX */
