//                                               -*- C++ -*-
/**
 *  @file  VisualTest.hxx
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
 *  Id      VisualTest.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_VISUALTEST_HXX
#define OPENTURNS_VISUALTEST_HXX

#include "OTprivate.hxx"
#include "NumericalSample.hxx"
#include "LinearModel.hxx"
#include "Distribution.hxx"
#include "Copula.hxx"
#include "DistributionImplementation.hxx"
#include "Normal.hxx"
#include "Graph.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class Test
 *
 */

class VisualTest
{
public:


  /** Default constructor, needed by SWIG */
  VisualTest();

  /** Draw the empirical CDF of the Sample when its dimension is 1 */
  static Graph DrawEmpiricalCDF(const NumericalSample & sample,
                                const NumericalScalar xMin,
                                const NumericalScalar xMax);

  /** Draw the Histogram of the Sample when its dimension is 1 */
  static Graph DrawHistogram(const NumericalSample & sample,
                             const UnsignedLong BarNumber);

  /** Draw the Histogram of the Sample when its dimension is 1, using the Normal empirical rule */
  static Graph DrawHistogram(const NumericalSample & sample);

  /** Draw the QQplot of two Samples when its dimension is 1 */
  static Graph DrawQQplot(const NumericalSample & sample1,
                          const NumericalSample & sample2,
                          const UnsignedLong pointNumber = ResourceMap::GetAsUnsignedLong("DistributionImplementation-DefaultPointNumber"));


  /** Draw the QQplot of one Sample and one Distribution when its dimension is 1 */
  static Graph DrawQQplot(const NumericalSample & sample,
                          const Distribution & dist,
                          const UnsignedLong pointNumber = ResourceMap::GetAsUnsignedLong("DistributionImplementation-DefaultPointNumber"));

  /** Draw the Henry curve for one Sample when its dimension is 1 */
  static Graph DrawHenryLine(const NumericalSample & sample);

  /** Draw the clouds of one Sample and one model when its dimension is 2 */
  static Graph DrawClouds(const NumericalSample & sample1,
                          const Distribution & dist);

  /** Draw the clouds of two Samples when its dimension is 2 */
  static Graph DrawClouds(const NumericalSample & sample1,
                          const NumericalSample & sample2);

  /** Draw the visual test for the LinearModel when its dimension is 1 */
  static Graph DrawLinearModel(const NumericalSample & sample1,
                               const NumericalSample & sample2,
                               const LinearModel & linearModel);

  /** Draw the visual test for the LinearModel residuals when its dimension is 1 */
  static Graph DrawLinearModelResidual(const NumericalSample & sample1,
                                       const NumericalSample & sample2,
                                       const LinearModel & linearModel);

  /** Draw the CobWeb visual test */
  static Graph DrawCobWeb(const NumericalSample & inputSample,
                          const NumericalSample & outputSample,
                          const NumericalScalar minValue,
                          const NumericalScalar maxValue,
                          const String & color,
                          const Bool quantileScale = true);

  /** Draw the Kendall plot to assess a copula for a bidimensional sample */
  static Graph DrawKendallPlot(const NumericalSample & data,
                               const Copula & copula);

  /** Draw the Kendall plot to assess if two bidimensional samples share the same copula */
  static Graph DrawKendallPlot(const NumericalSample & firstSample,
                               const NumericalSample & secondSample);

private:

  /** Compute the Kendall plot empirical statistic associated with a bidimensional sample */
  static NumericalSample ComputeKendallPlotEmpiricalStatistics(const NumericalSample & sample);

  /** Compute the Kendall plot theoretical statistic associated with a bidimensional copula */
  static NumericalSample ComputeKendallPlotTheoreticalStatistics(const Copula & copula,
                                                                 const UnsignedLong size);


}; /* class VisualTest */

END_NAMESPACE_OPENTURNS
#endif /* OPENTURNS_VISUALTEST_HXX */
