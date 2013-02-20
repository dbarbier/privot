//                                               -*- C++ -*-
/**
 *  @file  LinearModelTest.hxx
 *  @brief StatTest implements statistical tests
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 *  Id      LinearModelTest.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_LINEARMODELTEST_HXX
#define OPENTURNS_LINEARMODELTEST_HXX

#include "OTprivate.hxx"
#include "TestResult.hxx"
#include "NumericalSample.hxx"
#include "LinearModel.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class LinearModelTest
 *
 */

class LinearModelTest
{
public:


  /** Default constructor */
  LinearModelTest();

  /**  */
  static TestResult LinearModelAdjustedRSquared(const NumericalSample & firstSample,
                                                const NumericalSample & secondSample,
                                                const LinearModel & linearModel,
                                                const NumericalScalar level = 0.95);

  /**  */
  static TestResult LinearModelAdjustedRSquared(const NumericalSample & firstSample,
                                                const NumericalSample & secondSample,
                                                const NumericalScalar level = 0.95);

  /**  */
  static TestResult LinearModelFisher(const NumericalSample & firstSample,
                                      const NumericalSample & secondSample,
                                      const LinearModel & linearModel,
                                      const NumericalScalar level = 0.95);

  /**  */
  static TestResult LinearModelFisher(const NumericalSample & firstSample,
                                      const NumericalSample & secondSample,
                                      const NumericalScalar level = 0.95);

  /**  */
  static TestResult LinearModelResidualMean(const NumericalSample & firstSample,
                                            const NumericalSample & secondSample,
                                            const LinearModel & linearModel,
                                            const NumericalScalar level = 0.95);

  /**  */
  static TestResult LinearModelResidualMean(const NumericalSample & firstSample,
                                            const NumericalSample & secondSample,
                                            const NumericalScalar level = 0.95);

  /**  */
  static TestResult LinearModelRSquared(const NumericalSample & firstSample,
                                        const NumericalSample & secondSample,
                                        const LinearModel & linearModel,
                                        const NumericalScalar level = 0.95);

  /**  */
  static TestResult LinearModelRSquared(const NumericalSample & firstSample,
                                        const NumericalSample & secondSample,
                                        const NumericalScalar level = 0.95);

protected:
  /** Generic invocation of a R script for testing a distribution against a sample */
  static TestResult RunTwoSamplesALinearModelRTest(const NumericalSample & firstSample,
                                                   const NumericalSample & secondSample,
                                                   const LinearModel & linearModel,
                                                   const NumericalScalar level,
                                                   const String & testName);

}; /* class LinearModelTest */

END_NAMESPACE_OPENTURNS
#endif /* OPENTURNS_LINEARMODELTEST_HXX */
