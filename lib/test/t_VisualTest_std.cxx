//                                               -*- C++ -*-
/**
 *  @file  t_VisualTest_std.cxx
 *  @brief The test file of class VisualTest
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
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;

int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);
  setRandomGenerator();

  /* EmpiricalCDF tests */
  {
    /* Generate a Normal sample */
    Normal normal(1);
    UnsignedLong size(100);
    NumericalSample sample(normal.getSample(size));
    Graph sampleCDF(VisualTest::DrawEmpiricalCDF(sample, sample.getMin()[0] - 1.0, sample.getMax()[0] + 1.0));
    sampleCDF.draw("sampleCDF", 640, 480);
    fullprint << "bitmap = " << sampleCDF.getBitmap() << std::endl;
    fullprint << "postscript = " << sampleCDF.getPostscript() << std::endl;

    sampleCDF = VisualTest::DrawEmpiricalCDF(sample, -0.5, 0.5);
    sampleCDF.draw("sampleCDFZoom", 640, 480);
    fullprint << "bitmap = " << sampleCDF.getBitmap() << std::endl;
    fullprint << "postscript = " << sampleCDF.getPostscript() << std::endl;
  }

  /* Histogram tests */
  {
    Normal normal(1);
    UnsignedLong size(100);
    NumericalSample sample(normal.getSample(size));
    Graph sampleHist(VisualTest::DrawHistogram(sample, 10));
    sampleHist.draw("sampleHist", 640, 480);
    fullprint << "bitmap = " << sampleHist.getBitmap() << std::endl;
    fullprint << "postscript = " << sampleHist.getPostscript() << std::endl;

    sampleHist = VisualTest::DrawHistogram(sample);
    sampleHist.draw("sampleHistOpt", 640, 480);
    fullprint << "bitmap = " << sampleHist.getBitmap() << std::endl;
    fullprint << "postscript = " << sampleHist.getPostscript() << std::endl;
  }

  /* QQPlot tests */
  {
    UnsignedLong size(100);
    Normal normal(1);
    NumericalSample sample(normal.getSample(size));
    NumericalSample sample2(Gamma(3.0, 4.0, 0.0).getSample(size));
    Graph twoSamplesQQPlot(VisualTest::DrawQQplot(sample, sample2, 100));
    twoSamplesQQPlot.draw("twoSamplesQQPlot", 640, 480);
    fullprint << "bitmap = " << twoSamplesQQPlot.getBitmap() << std::endl;
    fullprint << "postscript = " << twoSamplesQQPlot.getPostscript() << std::endl;

    Graph sampleDistributionQQPlot(VisualTest::DrawQQplot(sample, normal));
    sampleDistributionQQPlot.draw("sampleDistributionQQPlot", 640, 480);
    fullprint << "bitmap = " << sampleDistributionQQPlot.getBitmap() << std::endl;
    fullprint << "postscript = " << sampleDistributionQQPlot.getPostscript() << std::endl;
  }

  /* HenryLine test */
  {
    UnsignedLong size(100);
    Normal normal(1);
    NumericalSample sample(normal.getSample(size));
    Graph henryPlot(VisualTest::DrawHenryLine(sample));
    henryPlot.draw("HenryPlot", 640, 480);
    fullprint << "bitmap = " << henryPlot.getBitmap() << std::endl;
    fullprint << "postscript = " << henryPlot.getPostscript() << std::endl;
  }

  /* Clouds tests */
  {
    UnsignedLong dimension(2);
    CorrelationMatrix R(dimension);
    R(0, 1) = 0.8;
    Normal distribution(NumericalPoint(dimension, 3.0), NumericalPoint(dimension, 2.0), R);
    UnsignedLong size(100);
    NumericalSample sample2D(distribution.getSample(size));
    NumericalSample firstSample(size, 1);
    NumericalSample secondSample(size, 1);
    for (UnsignedLong i = 0; i < size; i++)
      {
        firstSample[i][0] = sample2D[i][0];
        secondSample[i][0] = sample2D[i][1];
      }

    Graph sampleSampleClouds(VisualTest::DrawClouds(sample2D, Normal(NumericalPoint(dimension, 2.0), NumericalPoint(dimension, 3.0), R).getSample(size / 2)));
    sampleSampleClouds.draw("sampleSampleClouds", 640, 480);
    fullprint << "bitmap = " << sampleSampleClouds.getBitmap() << std::endl;
    fullprint << "postscript = " << sampleSampleClouds.getPostscript() << std::endl;

    Graph sampleDistributionClouds(VisualTest::DrawClouds(sample2D, Normal(NumericalPoint(dimension, 2.5), NumericalPoint(dimension, 1.0), R)));
    sampleDistributionClouds.draw("sampleDistributionClouds", 640, 480);
    fullprint << "bitmap = " << sampleDistributionClouds.getBitmap() << std::endl;
    fullprint << "postscript = " << sampleDistributionClouds.getPostscript() << std::endl;
  }

  /* LinearModel tests */
  {
    UnsignedLong dimension(2);
    CorrelationMatrix R(dimension);
    R(0, 1) = 0.8;
    Normal distribution(NumericalPoint(dimension, 3.0), NumericalPoint(dimension, 2.0), R);
    UnsignedLong size(100);
    NumericalSample sample2D(distribution.getSample(size));
    NumericalSample firstSample(size, 1);
    NumericalSample secondSample(size, 1);
    for (UnsignedLong i = 0; i < size; i++)
      {
        firstSample[i][0] = sample2D[i][0];
        secondSample[i][0] = sample2D[i][1];
      }

    LinearModel lmtest(LinearModelFactory().build(firstSample, secondSample));
    Graph drawLinearModelVTest(VisualTest::DrawLinearModel(firstSample, secondSample, lmtest));
    drawLinearModelVTest.draw("LinearModelV", 640, 480);
    fullprint << "bitmap = " << drawLinearModelVTest.getBitmap() << std::endl;
    fullprint << "postscript = " << drawLinearModelVTest.getPostscript() << std::endl;

    Graph drawLinearModelResidualTest(VisualTest::DrawLinearModelResidual(firstSample, secondSample, lmtest));
    drawLinearModelResidualTest.draw("LinearModelR", 640, 480);
    fullprint << "bitmap = " << drawLinearModelResidualTest.getBitmap() << std::endl;
    fullprint << "postscript = " << drawLinearModelResidualTest.getPostscript() << std::endl;
  }

  /* CobWeb tests */
  {
    UnsignedLong size(100);
    UnsignedLong inputDimension(6);
    NumericalSample inputSample(Normal(inputDimension).getSample(size));
    Description inputVar(inputDimension);
    for (UnsignedLong i = 0; i < inputDimension; ++i) inputVar[i] = (OSS() << "X" << i);
    Description formula(1);
    OSS oss;
    for (UnsignedLong i = 0; i < inputDimension; ++i) oss << (i > 0 ? "+" : "") << "cos(" << i + 1 << "*" << inputVar[i] << ")";
    formula[0] = oss;
    Description outputVar(1);
    outputVar[0] = "y";
    NumericalMathFunction model(inputVar, outputVar, formula);
    NumericalSample outputSample(model(inputSample));
    Graph cobwebValue(VisualTest::DrawCobWeb(inputSample, outputSample, 2.5, 3.0, "red", false));
    cobwebValue.draw("cobwebValue", 640, 480);
    fullprint << "bitmap = " << cobwebValue.getBitmap() << std::endl;
    fullprint << "postscript = " << cobwebValue.getPostscript() << std::endl;
    Graph cobwebQuantile(VisualTest::DrawCobWeb(inputSample, outputSample, 0.7, 0.9, "red", false));
    cobwebQuantile.draw("cobwebQuantile", 640, 480);
    fullprint << "bitmap = " << cobwebQuantile.getBitmap() << std::endl;
    fullprint << "postscript = " << cobwebQuantile.getPostscript() << std::endl;
  }

  /* KendallPlot tests */
  {
    UnsignedLong size(100);
    FrankCopula copula1(1.5);
    GumbelCopula copula2(4.5);
    NumericalSample sample1(copula1.getSample(size));
    sample1.setName("data 1");
    NumericalSample sample2(copula2.getSample(size));
    sample2.setName("data 2");
    Graph kendallPlot1(VisualTest::DrawKendallPlot(sample1, copula2));
    kendallPlot1.draw("KendallPlot1", 640, 480);
    fullprint << "bitmap = " << kendallPlot1.getBitmap() << std::endl;
    fullprint << "postscript = " << kendallPlot1.getPostscript() << std::endl;
    Graph kendallPlot2(VisualTest::DrawKendallPlot(sample2, sample1));
    kendallPlot2.draw("KendallPlot2", 640, 480);
    fullprint << "bitmap = " << kendallPlot2.getBitmap() << std::endl;
    fullprint << "postscript = " << kendallPlot2.getPostscript() << std::endl;
  }

  return ExitCode::Success;
}
