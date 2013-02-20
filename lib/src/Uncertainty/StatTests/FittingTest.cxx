//                                               -*- C++ -*-
/**
 *  @file  FittingTest.cxx
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
 *  @author schueller
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 */
#include <cmath>
#include <fstream>
#include "FittingTest.hxx"
#include "NumericalPoint.hxx"
#include "Description.hxx"
#include "Path.hxx"
#include "ResourceMap.hxx"
#include "Log.hxx"
#include "Os.hxx"
#include "SpecFunc.hxx"
#include "DistFunc.hxx"
#include "OTconfig.hxx"

BEGIN_NAMESPACE_OPENTURNS

TestResult FittingTest::lastResult_ = TestResult();

/* Default constructor, needed by SWIG */
FittingTest::FittingTest()
{
  // Nothing to do
}

/* Best model for a given numerical sample by BIC */
Distribution FittingTest::BestModelBIC(const NumericalSample & sample,
                                       const DistributionFactoryCollection & factoryCollection)
{
  const UnsignedLong size(factoryCollection.getSize());
  if (size == 0) throw InternalException(HERE) << "Error: no model given";
  Distribution bestDistribution;
  NumericalScalar bestConcordanceMeasure(SpecFunc::MaxNumericalScalar);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const DistributionFactory factory(factoryCollection[i]);
      try
        {
          LOGINFO(OSS(false) << "Trying factory " << factory);
          const Distribution distribution(factory.build(sample));
          const NumericalScalar concordanceMeasure(BIC(sample, distribution, distribution.getParametersNumber()));
          LOGINFO(OSS(false) << "Resulting distribution=" << distribution << ", BIC=" << concordanceMeasure);
          if (concordanceMeasure < bestConcordanceMeasure)
            {
              bestConcordanceMeasure = concordanceMeasure;
              bestDistribution = distribution;
            }
        }
      catch (InvalidArgumentException & ex)
        {
          LOGWARN(OSS(false) << "Warning! Impossible to use factory " << factory << ". Reason=" << ex);
        }
    }
  if (bestConcordanceMeasure == SpecFunc::MaxNumericalScalar) LOGWARN(OSS(false) << "Be carefull, the best model has an infinite concordance measure. The output distribution must be severely wrong.");
  return bestDistribution;
}

/* Best model for a given numerical sample by BIC */
Distribution FittingTest::BestModelBIC(const NumericalSample  & sample,
                                       const DistributionCollection & distributionCollection)
{
  const UnsignedLong size(distributionCollection.getSize());
  if (size == 0) throw InternalException(HERE) << "Error: no model given";
  Distribution bestDistribution;
  NumericalScalar bestConcordanceMeasure(SpecFunc::MaxNumericalScalar);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const Distribution distribution(distributionCollection[i]);
      LOGINFO(OSS(false) << "Testing distribution " << distribution);
      const NumericalScalar concordanceMeasure(BIC(sample, distribution));
      LOGINFO(OSS(false) << "BIC=" << concordanceMeasure);
      if (concordanceMeasure < bestConcordanceMeasure)
        {
          bestConcordanceMeasure = concordanceMeasure;
          bestDistribution = distribution;
        }
    }
  if (bestConcordanceMeasure > SpecFunc::MaxNumericalScalar) LOGWARN(OSS(false) << "Be carefull, the best model has an infinite concordance measure. The output distribution must be severely wrong.");
  return bestDistribution;
}




/* Best model for a given numerical sample by Kolmogorov */
Distribution FittingTest::BestModelKolmogorov(const NumericalSample & sample,
                                              const DistributionFactoryCollection & factoryCollection)
{
  const UnsignedLong size(factoryCollection.getSize());
  if (size == 0) throw InternalException(HERE) << "Error: no model given";
  const NumericalScalar fakeLevel(0.5);
  Distribution bestDistribution;
  TestResult bestResult;
  NumericalScalar bestPValue(0.0);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const DistributionFactory factory(factoryCollection[i]);
      try
        {
          LOGINFO(OSS(false) << "Trying factory " << factory);
          const Distribution distribution(factoryCollection[i].build(sample));
          const TestResult result(Kolmogorov(sample, distribution, fakeLevel, distribution.getParametersNumber()));
          LOGINFO(OSS(false) << "Resulting distribution=" << distribution << ", test result=" << result);
          if (result.getPValue() > bestPValue)
            {
              bestPValue = result.getPValue();
              bestResult = result;
              bestDistribution = distribution;
            }
        }
      catch (InvalidArgumentException & ex)
        {
          LOGWARN(OSS(false) << "Warning! Impossible to use factory " << factory << ". Reason=" << ex);
        }
    }
  if ( bestPValue == 0.0) LOGWARN(OSS(false) << "Be carefull, the best model has a p-value of zero. The output distribution must be severely wrong.");
  lastResult_ = bestResult;
  return bestDistribution;
}

/* Best model for a given numerical sample by Kolmogorov */
Distribution FittingTest::BestModelKolmogorov(const NumericalSample & sample,
                                              const DistributionCollection & distributionCollection)
{
  const UnsignedLong size(distributionCollection.getSize());
  if (size == 0) throw InternalException(HERE) << "Error: no model given";
  Distribution bestDistribution;
  TestResult bestResult;
  NumericalScalar bestPValue(0.0);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const Distribution distribution(distributionCollection[i]);
      LOGINFO(OSS(false) << "Testing distribution " << distribution);
      const TestResult result(Kolmogorov(sample, distribution));
      LOGINFO(OSS(false) << "Test result=" << result);
      if (result.getPValue() > bestPValue)
        {
          bestPValue = result.getPValue();
          bestResult = result;
          bestDistribution = distribution;
        }
    }
  if ( bestPValue == 0.0) LOGWARN(OSS(false) << "Be carefull, the best model has a p-value of zero. The output distribution must be severely wrong.");
  lastResult_ = bestResult;
  return bestDistribution;
}


/* Best model for a given numerical sample by ChiSquared */
Distribution FittingTest::BestModelChiSquared(const NumericalSample & sample,
                                              const DistributionFactoryCollection & factoryCollection)
{
  const UnsignedLong size(factoryCollection.getSize());
  if (size == 0) throw InternalException(HERE) << "Error: no model given";
  const NumericalScalar fakeLevel(0.5);
  Distribution bestDistribution(factoryCollection[0].build(sample));
  TestResult bestResult(ChiSquared(sample, bestDistribution, fakeLevel, bestDistribution.getParametersNumber()));
  for (UnsignedLong i = 1; i < size; ++i)
    {
      const Distribution distribution(factoryCollection[i].build(sample));
      const TestResult result(ChiSquared(sample, distribution, fakeLevel, distribution.getParametersNumber()));
      if (result.getPValue() > bestResult.getPValue())
        {
          bestResult = result;
          bestDistribution = distribution;
        }
    }
  if ( bestResult.getPValue() == 0.0) LOGWARN(OSS(false) << "Be carefull, the best model has a p-value of zero.");
  lastResult_ = bestResult;
  return bestDistribution;
}

/* Best model for a given numerical sample by ChiSquared */
Distribution FittingTest::BestModelChiSquared(const NumericalSample & sample,
                                              const DistributionCollection & distributionCollection)
{
  const UnsignedLong size(distributionCollection.getSize());
  if (size == 0) throw InternalException(HERE) << "Error: no model given";
  Distribution bestDistribution(distributionCollection[0]);
  TestResult bestResult(ChiSquared(sample, bestDistribution));
  for (UnsignedLong i = 1; i < size; ++i)
    {
      const Distribution distribution(distributionCollection[i]);
      const TestResult result(ChiSquared(sample, distribution));
      if (result.getPValue() > bestResult.getPValue())
        {
          bestResult = result;
          bestDistribution = distribution;
        }
    }
  if ( bestResult.getPValue() == 0.0) LOGWARN(OSS(false) << "Be carefull, the best model has a p-value of zero.");
  lastResult_ = bestResult;
  return bestDistribution;
}

/* Bayesian Information Criterion computation */
NumericalScalar FittingTest::BIC(const NumericalSample & sample,
                                 const Distribution & distribution,
                                 const UnsignedLong estimatedParameters)
{
  if (sample.getDimension() != distribution.getDimension()) throw InvalidArgumentException(HERE) << "Error: the sample dimension and the distribution dimension must be equal";
  const UnsignedLong size(sample.getSize());
  const UnsignedLong parametersNumber(distribution.getParametersNumber());
  if (parametersNumber < estimatedParameters) throw InvalidArgumentException(HERE) << "Error: the number of estimated parameters cannot exceed the number of parameters of the distribution";
  NumericalScalar logLikelihood(0.0);
  for (UnsignedLong i = 0; i < size; ++i) logLikelihood += log(distribution.computePDF(sample[i]));
  lastResult_ = TestResult();
  return (-2.0 * logLikelihood + estimatedParameters * log(size)) / size;
}

/* Bayesian Information Criterion computation */
NumericalScalar FittingTest::BIC(const NumericalSample & sample,
                                 const DistributionFactory & factory)
{
  const Distribution distribution(factory.build(sample));
  return BIC(sample, distribution, distribution.getParametersNumber());
}


/* Kolmogorov test */
TestResult FittingTest::Kolmogorov(const NumericalSample & sample,
                                   const DistributionFactory & factory,
                                   const NumericalScalar level)
{
  if ((level <= 0.0) || (level >= 1.0)) throw InvalidArgumentException(HERE) << "Error: level must be in ]0, 1[, here level=" << level;
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: Kolmogorov test works only with 1D samples";
  const Distribution distribution(factory.build(sample));
  if (!distribution.getImplementation()->isContinuous()) throw InvalidArgumentException(HERE) << "Error: Kolmogorov test can be applied only to a continuous distribution";
  if (distribution.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: Kolmogorov test works only with 1D distribution";
  return Kolmogorov(sample, distribution, level, distribution.getParametersNumber());
}


/* Kolmogorov test */
TestResult FittingTest::Kolmogorov(const NumericalSample & sample,
                                   const Distribution & distribution,
                                   const NumericalScalar level,
                                   const UnsignedLong estimatedParameters)
{
  if ((level <= 0.0) || (level >= 1.0)) throw InvalidArgumentException(HERE) << "Error: level must be in ]0, 1[, here level=" << level;
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: Kolmogorov test works only with 1D samples";
  if (!distribution.getImplementation()->isContinuous()) throw InvalidArgumentException(HERE) << "Error: Kolmogorov test can be applied only to a continuous distribution";
  if (distribution.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: Kolmogorov test works only with 1D distribution";
  if (estimatedParameters > 0) LOGINFO("Warning: using Kolmogorov test for a distribution with estimated parameters will result in an overestimated pValue");
  const NumericalSample sortedSample(sample.sort(0));
  const UnsignedLong size(sample.getSize());
  NumericalScalar value(0.0);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalScalar cdfValue(distribution.computeCDF(sortedSample[i]));
      value = std::max(value, std::max(fabs(NumericalScalar(i) / size - cdfValue), fabs(cdfValue - NumericalScalar(i + 1) / size)));
    }
  const NumericalScalar pValue(DistFunc::pKolmogorov(size, value, true));
  lastResult_ = TestResult(OSS(false) << "Kolmogorov" << distribution.getClassName(), (pValue > 1.0 - level), pValue, 1.0 - level);
  return GetLastResult();
}

/* Chi-squared test */
TestResult FittingTest::ChiSquared(const NumericalSample & sample,
                                   const DistributionFactory & factory,
                                   const NumericalScalar level)
{
  if ((level <= 0.0) || (level >= 1.0)) throw InvalidArgumentException(HERE) << "Error: level must be in ]0, 1[, here level=" << level;
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: ChiSquared test works only with 1D samples";
  const Distribution distribution(factory.build(sample));
  if (distribution.getImplementation()->isContinuous()) throw InvalidArgumentException(HERE) << "Error: Chi-squared test cannot be applied to a continuous distribution";
  if (distribution.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: ChiSquared test works only with 1D distribution";
  return ChiSquared(sample, distribution, level, distribution.getParametersNumber());
}


/* Chi-squared test */
TestResult FittingTest::ChiSquared(const NumericalSample & sample,
                                   const Distribution & distribution,
                                   const NumericalScalar level,
                                   const UnsignedLong estimatedParameters)
{
  if ((level <= 0.0) || (level >= 1.0)) throw InvalidArgumentException(HERE) << "Error: level must be in ]0, 1[, here level=" << level;
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: ChiSquared test works only with 1D samples";
  if (distribution.getImplementation()->isContinuous()) throw InvalidArgumentException(HERE) << "Error: Chi-squared test cannot be applied to a continuous distribution";
  if (distribution.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: ChiSquared test works only with 1D distribution";
  return RunRTest(sample, distribution, level, estimatedParameters, "ChiSquared");
}

/* Generic invocation of a R script for testing a distribution against a sample */
TestResult FittingTest::RunRTest(const NumericalSample & sample,
                                 const Distribution & distribution,
                                 const NumericalScalar level,
                                 const UnsignedLong estimatedParameters,
                                 const String & testName)
{
  const String dataFileName(sample.storeToTemporaryFile());
  const String resultFileName(Path::BuildTemporaryFileName("RResult.txt.XXXXXX"));
  const String commandFileName(Path::BuildTemporaryFileName("RCmd.R.XXXXXX"));
  std::ofstream cmdFile(commandFileName.c_str(), std::ios::out);
  // Fill-in the command file
  cmdFile << "library(rot)" << std::endl;
  cmdFile << "options(digits=17)" << std::endl;
  cmdFile << "options(warn=-1)" << std::endl;
  cmdFile << "sample <- data.matrix(read.table(\"" << dataFileName << "\"))" << std::endl;
  cmdFile << "res <- computeTest" << testName << distribution.getImplementation()->getClassName();
  cmdFile << "(sample, ";
  const NumericalPoint parameters(distribution.getParametersCollection()[0]);
  const UnsignedLong parametersNumber(parameters.getDimension());
  for (UnsignedLong i = 0; i < parametersNumber; ++i) cmdFile << parameters[i] << ", ";
  cmdFile << level << ", " << estimatedParameters << ")" << std::endl;
  cmdFile << "f <- file(\"" << resultFileName << "\",\"wt\")" << std::endl;
  cmdFile << "cat(res$test, res$testResult, res$threshold, res$pValue, sep=\"\\n\", file=f)" << std::endl;
  cmdFile << "close(f)" << std::endl;
  cmdFile.close();
  const String RExecutable(ResourceMap::Get("R-executable-command"));
  OSS systemCommand;
  if (RExecutable != "") systemCommand << RExecutable << " --no-save --silent < \"" << commandFileName << "\"" << Os::GetDeleteCommandOutput();
  else throw NotYetImplementedException(HERE) << "FittingTest::RunRTest() needs R. Please install it and set the absolute path of the R executable in ResourceMap.";
  const int returnCode(Os::ExecuteCommand(systemCommand));
  if (returnCode != 0) throw InternalException(HERE) << "Error: unable to execute the system command " << String(systemCommand) << " returned code is " << returnCode;
  // Parse result file
  std::ifstream resultFile(resultFileName.c_str(), std::ios::in);
  String testType;
  resultFile >> testType;
  Bool testResult;
  resultFile >> testResult;
  NumericalScalar pThreshold;
  resultFile >> pThreshold;
  NumericalScalar pValue;
  resultFile >> pValue;

  // Clean-up everything
  if (remove(dataFileName.c_str()) == -1) LOGWARN(OSS(false) << "Warning: cannot remove file " << dataFileName);
  if (remove(resultFileName.c_str()) == -1) LOGWARN(OSS(false) << "Warning: cannot remove file " << dataFileName);
  if (remove(commandFileName.c_str()) == -1) LOGWARN(OSS(false) << "Warning: cannot remove file " << dataFileName);

  lastResult_ = TestResult(testType, testResult, pValue, pThreshold);
  return GetLastResult();
}

/* Get last fitting result */
TestResult FittingTest::GetLastResult()
{
  return lastResult_;
}

END_NAMESPACE_OPENTURNS
