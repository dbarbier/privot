//                                               -*- C++ -*-
/**
 *  @file  t_Histogram_std.cxx
 *  @brief The test file of class Histogram for standard methods
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

  try
    {
      // Instanciate one distribution object
      UnsignedLong collectionSize(4);
      Histogram::HistogramPairCollection collection(collectionSize);
      collection[0] = HistogramPair(1.0, 0.5);
      collection[1] = HistogramPair(0.7, 1.5);
      collection[2] = HistogramPair(1.2, 3.5);
      collection[3] = HistogramPair(0.9, 2.5);
      fullprint << "collection=" << collection << std::endl;
      Histogram distribution(-1.5, collection);
      fullprint << "Distribution " << distribution << std::endl;
      std::cout << "Distribution " << distribution << std::endl;

      // Is this distribution elliptical ?
      fullprint << "Elliptical = " << (distribution.isElliptical() ? "true" : "false") << std::endl;

      // Is this distribution continuous ?
      fullprint << "Continuous = " << (distribution.isContinuous() ? "true" : "false") << std::endl;

      // Test for realization of distribution
      NumericalPoint oneRealization = distribution.getRealization();
      fullprint << "oneRealization=" << oneRealization << std::endl;

      // Test for sampling
      UnsignedLong size = 10000;
      NumericalSample oneSample = distribution.getSample( size );
      fullprint << "oneSample first=" << oneSample[0] << " last=" << oneSample[size - 1] << std::endl;
      fullprint << "mean=" << oneSample.computeMean() << std::endl;
      fullprint << "covariance=" << oneSample.computeCovariance() << std::endl;

      // Define a point
      NumericalPoint point( distribution.getDimension(), 1.0 );
      fullprint << "Point= " << point << std::endl;

      // Show PDF and CDF of point
      NumericalScalar eps(1e-5);
      NumericalPoint DDF = distribution.computeDDF( point );
      fullprint << "ddf     =" << DDF << std::endl;
      fullprint << "ddf (FD)=" << distribution.ContinuousDistribution::computeDDF(point) << std::endl;
      NumericalScalar LPDF = distribution.computeLogPDF( point );
      fullprint << "log pdf=" << LPDF << std::endl;
      NumericalScalar PDF = distribution.computePDF( point );
      fullprint << "pdf     =" << PDF << std::endl;
      fullprint << "pdf (FD)=" << (distribution.computeCDF( point + NumericalPoint(1, eps) ) - distribution.computeCDF( point  + NumericalPoint(1, -eps) )) / (2.0 * eps) << std::endl;
      NumericalScalar CDF = distribution.computeCDF( point );
      fullprint << "cdf=" << CDF << std::endl;
      NumericalScalar CCDF = distribution.computeComplementaryCDF( point );
      fullprint << "ccdf=" << CCDF << std::endl;
      //    NumericalPoint PDFgr = distribution.computePDFGradient( point );
      //    fullprint << "pdf gradient     =" << PDFgr << std::endl;
      //    NumericalPoint PDFgrFD(1 + 2 * collectionSize);
      //    PDFgrFD[0] = (Histogram(distribution.getFirst() + eps, distribution.getPairCollection()).computePDF(point) -
      //                  Histogram(distribution.getFirst() - eps, distribution.getPairCollection()).computePDF(point)) / (2.0 * eps);
      //    for (UnsignedLong i = 0; i < collectionSize; i++)
      //      {
      //  Histogram::PairCollection collectionLeft = distribution.getPairCollection();
      //  Histogram::PairCollection collectionRight = distribution.getPairCollection();
      //  collectionLeft[i].h_  += eps;
      //  collectionRight[i].h_ -= eps;
      //  PDFgrFD[2 * i + 1] = (Histogram(distribution.getFirst(), collectionLeft).computePDF(point) -
      //                        Histogram(distribution.getFirst(), collectionRight).computePDF(point)) / (2.0 * eps);
      //  collectionLeft = distribution.getPairCollection();
      //  collectionRight = collectionLeft;
      //  collectionLeft[i].l_  += eps;
      //  collectionRight[i].l_ -= eps;
      //  PDFgrFD[2 * i + 2] = (Histogram(distribution.getFirst(), collectionLeft).computePDF(point) -
      //                        Histogram(distribution.getFirst(), collectionRight).computePDF(point)) / (2.0 * eps);
      //      }
      //    fullprint << "pdf gradient (FD)=" << PDFgrFD << std::endl;
      //    NumericalPoint CDFgr = distribution.computeCDFGradient( point );
      //    fullprint << "cdf gradient     =" << CDFgr << std::endl;
      //    NumericalPoint CDFgrFD(1 + 2 * collectionSize);
      //    CDFgrFD[0] = (Histogram(distribution.getFirst() + eps, distribution.getPairCollection()).computeCDF(point) -
      //                  Histogram(distribution.getFirst() - eps, distribution.getPairCollection()).computeCDF(point)) / (2.0 * eps);
      //    for (UnsignedLong i = 0; i < collectionSize; i++)
      //      {
      //  Histogram::PairCollection collectionLeft = distribution.getPairCollection();
      //  Histogram::PairCollection collectionRight = distribution.getPairCollection();
      //  collectionLeft[i].h_  += eps;
      //  collectionRight[i].h_ -= eps;
      //  CDFgrFD[2 * i + 1] = (Histogram(distribution.getFirst(), collectionLeft).computeCDF(point) -
      //                        Histogram(distribution.getFirst(), collectionRight).computeCDF(point)) / (2.0 * eps);
      //  collectionLeft = distribution.getPairCollection();
      //  collectionRight = collectionLeft;
      //  collectionLeft[i].l_  += eps;
      //  collectionRight[i].l_ -= eps;
      //  CDFgrFD[2 * i + 2] = (Histogram(distribution.getFirst(), collectionLeft).computeCDF(point) -
      //                        Histogram(distribution.getFirst(), collectionRight).computeCDF(point)) / (2.0 * eps);
      //      }
      //    fullprint << "cdf gradient (FD)=" << CDFgrFD << std::endl;
      NumericalPoint quantile = distribution.computeQuantile( 0.95 );
      fullprint << "quantile=" << quantile << std::endl;
      fullprint << "cdf(quantile)=" << distribution.computeCDF(quantile) << std::endl;
      NumericalPoint mean = distribution.getMean();
      fullprint << "mean=" << mean << std::endl;
      CovarianceMatrix covariance = distribution.getCovariance();
      fullprint << "covariance=" << covariance << std::endl;
      Histogram::NumericalPointWithDescriptionCollection parameters = distribution.getParametersCollection();
      fullprint << "parameters=" << parameters << std::endl;
      for (UnsignedLong i = 0; i < 6; ++i) fullprint << "standard moment n=" << i << ", value=" << distribution.getStandardMoment(i) << std::endl;
      fullprint << "Standard representative=" << distribution.getStandardRepresentative()->__str__() << std::endl;

      UnsignedLong testSize(0);
      for (UnsignedLong i = 0; i < testSize; i++)
        {
          NumericalScalar q(RandomGenerator::Generate());
          if (fabs(q - distribution.computeCDF(distribution.computeQuantile(q))) > eps)
            {
              fullprint << "q=" << q << " quantile=" << distribution.computeQuantile(q)[0] << " CDF(quantile)=" << distribution.computeCDF(distribution.computeQuantile(q)) << std::endl;
            }
        }

      // Specific to this distribution
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
