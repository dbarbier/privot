//                                               -*- C++ -*-
/**
 *  @file  FittingTest.hxx
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 *  Id      FittingTest.hxx 2600 2012-07-16 08:12:54Z schueller
 */
#ifndef OPENTURNS_FITTINGTEST_HXX
#define OPENTURNS_FITTINGTEST_HXX

#include "OTprivate.hxx"
#include "TestResult.hxx"
#include "Collection.hxx"
#include "NumericalSample.hxx"
#include "Distribution.hxx"
#include "DistributionFactory.hxx"
#include "Pointer.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class FittingTest
 *
 */

class FittingTest
{
public:

  typedef Collection<DistributionFactory> DistributionFactoryCollection;
  typedef Collection<Distribution>        DistributionCollection;

  /** Default constructor, needed by SWIG */
  FittingTest();

  /** Best model for a given numerical sample by BIC */
  static Distribution BestModelBIC(const NumericalSample  & sample,
                                   const DistributionFactoryCollection & factoryCollection);

  /** Best model for a given numerical sample by BIC */
  static Distribution BestModelBIC(const NumericalSample  & sample,
                                   const DistributionCollection & distributionCollection);


  /** Best model for a given numerical sample by Kolmogorov */
  static Distribution BestModelKolmogorov(const NumericalSample  & sample,
                                          const DistributionFactoryCollection & factoryCollection);

  /** Best model for a given numerical sample by Kolmogorov */
  static Distribution BestModelKolmogorov(const NumericalSample  & sample,
                                          const DistributionCollection & distributionCollection);


  /** Best model for a given numerical sample by ChiSquared */
  static Distribution BestModelChiSquared(const NumericalSample  & sample,
                                          const DistributionFactoryCollection & factoryCollection);

  /** Best model for a given numerical sample by ChiSquared */
  static Distribution BestModelChiSquared(const NumericalSample  & sample,
                                          const DistributionCollection & distributionCollection);


  /** Bayesian Information Criterion computation */
  static NumericalScalar BIC(const NumericalSample & sample,
                             const Distribution & distribution,
                             const UnsignedLong estimatedParameters = 0);

  /** Bayesian Information Criterion computation */
  static NumericalScalar BIC(const NumericalSample & sample,
                             const DistributionFactory & factory);

  /** Kolmogorov fitting test for continuous distributions */
  static TestResult Kolmogorov(const NumericalSample & sample,
                               const Distribution & distribution,
                               const NumericalScalar level = 0.95,
                               const UnsignedLong estimatedParameters = 0);

  /** Kolmogorov fitting test for continuous distributions */
  static TestResult Kolmogorov(const NumericalSample & sample,
                               const DistributionFactory & factory,
                               const NumericalScalar level = 0.95);

  /** ChiSquared fitting test for discrete distributions */
  static TestResult ChiSquared(const NumericalSample & sample,
                               const Distribution & distribution,
                               const NumericalScalar level = 0.95,
                               const UnsignedLong estimatedParameters = 0);

  /** ChiSquared fitting test for discrete distributions */
  static TestResult ChiSquared(const NumericalSample & sample,
                               const DistributionFactory & factory,
                               const NumericalScalar level = 0.95);

  /** Get last fitting measure */
  static TestResult GetLastResult();

protected:
  /** Generic invocation of a R script for testing a distribution against a sample */
  static TestResult RunRTest(const NumericalSample & sample,
                             const Distribution & distribution,
                             const NumericalScalar level,
                             const UnsignedLong estimatedParameters,
                             const String & testName);

private:
  /** Stores last measure of test */
  static TestResult lastResult_;

}; /* class FittingTest */

END_NAMESPACE_OPENTURNS
#endif /* OPENTURNS_FITTINGTEST_HXX */
