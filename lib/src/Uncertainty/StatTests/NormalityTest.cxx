//                                               -*- C++ -*-
/**
 *  @file  NormalityTest.cxx
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */
#include <cmath>
#include <fstream>
#include "NormalityTest.hxx"
#include "Exception.hxx"
#include "Path.hxx"
#include "DistFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS

/* Default constructor */
NormalityTest::NormalityTest()
{
  // Nothing to do
}

/* Anderson Darling normality test for normal distributions. */
TestResult NormalityTest::AndersonDarlingNormal(const NumericalSample & sample,
                                                const NumericalScalar level)
{
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: cannot perform an Anderson Darling normality test with sample of dimension > 1.";
  if (sample.getSize() < 8) throw InvalidArgumentException(HERE) << "Error: cannot perform an Anderson Darling normality test with sample of size < 8.";
  NumericalSample sortedSample(sample.sort());
  NumericalScalar mean(sortedSample.computeMean()[0]);
  NumericalScalar sd(sortedSample.computeStandardDeviationPerComponent()[0]);
  const UnsignedLong size(sample.getSize());
  NumericalScalar testStatistic(0.0);
  UnsignedLong effectiveIndex(0);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      NumericalScalar yi(DistFunc::pNormal((sortedSample[i][0] - mean) / sd));
      NumericalScalar yni(1.0 - DistFunc::pNormal((sortedSample[size - i - 1][0] - mean) / sd));
      if ((yi > 0.0) && (yni > 0.0))
        {
          testStatistic += (2.0 * effectiveIndex + 1) * (log(yi) + log(yni));
          ++effectiveIndex;
        }
    }
  testStatistic /= effectiveIndex;
  testStatistic = (-NumericalScalar(effectiveIndex) - testStatistic);
  // Corrective factor for small sample size
  NumericalScalar adjustedStatistic(testStatistic * (1.0 + 0.75 / effectiveIndex + 2.25 / (effectiveIndex * effectiveIndex)));
  // Compute approximate p-value
  NumericalScalar pValue(1.0);
  if (adjustedStatistic >= -1.38)
    {
      pValue = 1 - exp(-13.436 + 101.14 * adjustedStatistic - 223.73 * adjustedStatistic * adjustedStatistic);
    }
  if (adjustedStatistic >= 0.2)
    {
      pValue = 1 - exp(-8.318 + 42.796 * adjustedStatistic - 59.938 * adjustedStatistic * adjustedStatistic);
    }
  if (adjustedStatistic >= 0.34)
    {
      pValue = exp(0.9177 - 4.279 * adjustedStatistic - 1.38 * adjustedStatistic * adjustedStatistic);
    }
  if (adjustedStatistic >= 0.6)
    {
      pValue = exp(1.2937 - 5.709 * adjustedStatistic + 0.0186 * adjustedStatistic * adjustedStatistic);
    }
  if (adjustedStatistic >= 153.0)
    {
      pValue = 0.0;
    }
  return TestResult("AndersonDarlingNormal", pValue > 1.0 - level, pValue, 1.0 - level);
}

/* Cramer Von Mises normality test for normal distributions. */
TestResult NormalityTest::CramerVonMisesNormal(const NumericalSample & sample,
                                               const NumericalScalar level)
{
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: cannot perform a Cramer Von-Mises normality test with sample of dimension > 1.";
  if (sample.getSize() < 8) throw InvalidArgumentException(HERE) << "Error: cannot perform a Cramer Von-Mises normality test with sample of size < 8.";
  NumericalSample sortedSample(sample.sort());
  NumericalScalar mean(sortedSample.computeMean()[0]);
  NumericalScalar sd(sortedSample.computeStandardDeviationPerComponent()[0]);
  const UnsignedLong size(sample.getSize());
  NumericalScalar testStatistic(1.0 / (12.0 * size));
  for (UnsignedLong i = 0; i < size; ++i)
    {
      NumericalScalar yi(DistFunc::pNormal((sortedSample[i][0] - mean) / sd));
      NumericalScalar delta(yi - (2.0 * i + 1.0) / (2.0 * size));
      testStatistic += delta * delta;
    }
  // Corrective factor for small sample size
  NumericalScalar adjustedStatistic(testStatistic * (1.0 + 0.5 / size));
  // Compute approximate p-value
  NumericalScalar pValue(1.0);
  if (adjustedStatistic >= -0.2)
    {
      pValue = 1 - exp(-13.953 + 775.5 * adjustedStatistic - 12542.61 * adjustedStatistic * adjustedStatistic);
    }
  if (adjustedStatistic >= 0.0275)
    {
      pValue = 1 - exp(-5.903 + 179.546 * adjustedStatistic - 1515.29 * adjustedStatistic * adjustedStatistic);
    }
  if (adjustedStatistic >= 0.051)
    {
      pValue = exp(0.886 - 31.62 * adjustedStatistic + 10.897 * adjustedStatistic * adjustedStatistic);
    }
  if (adjustedStatistic >= 0.092)
    {
      pValue = exp(1.111 - 34.242 * adjustedStatistic + 12.832 * adjustedStatistic * adjustedStatistic);
    }
  if (adjustedStatistic >= 2.636)
    {
      pValue = 0.0;
    }
  return TestResult("CramerVonMisesNormal", pValue > 1.0 - level, pValue, 1.0 - level);
}

END_NAMESPACE_OPENTURNS
