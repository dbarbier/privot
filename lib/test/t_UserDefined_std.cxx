//                                               -*- C++ -*-
/**
 *  @file  t_UserDefined_std.cxx
 *  @brief The test file of class UserDefined for standard methods
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
      typedef UserDefined::UserDefinedPairCollection PairCollection;
      PairCollection collection(3);
      NumericalPoint point(1);
      point[0] = 1.0;
      collection[0] = UserDefinedPair(point, 0.3);
      point[0] = 2.0;
      collection[1] = UserDefinedPair(point, 0.1);
      point[0] = 3.0;
      collection[2] = UserDefinedPair(point, 0.6);
      UserDefined distribution(collection);
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
      UnsignedLong size = 10;
      NumericalSample oneSample = distribution.getSample( size );
      fullprint << "oneSample=" << oneSample << std::endl;

      // Define a point
      point = NumericalPoint( distribution.getDimension(), 2.0 );

      // Show PDF and CDF of a point
      NumericalScalar pointPDF = distribution.computePDF( point );
      NumericalScalar pointCDF = distribution.computeCDF( point );
      fullprint << "point= " << point
                << " pdf=" << pointPDF
                << " cdf=" << pointCDF
                << std::endl;

      // Get 95% quantile
      NumericalPoint quantile = distribution.computeQuantile( 0.95 );
      fullprint << "Quantile=" << quantile << std::endl;

      for (UnsignedLong i = 0; i < 6; ++i) fullprint << "standard moment n=" << i << ", value=" << distribution.getStandardMoment(i) << std::endl;
      fullprint << "Standard representative=" << distribution.getStandardRepresentative()->__str__() << std::endl;

      NumericalSample sample(4, 3);
      for (UnsignedLong i = 0; i < 4; ++i)
        {
          for (UnsignedLong j = 0; j < 3; ++j)
            {
              sample[i][j] = 10 * (i + 1) + 0.1 * (j + 1);
            }
        }
      UserDefined multivariateUserDefined(sample);
      fullprint << "Multivariate UserDefined=" << multivariateUserDefined << std::endl;
      fullprint << "Marginal 0=" << Distribution(multivariateUserDefined.getMarginal(0)) << std::endl;
      Indices indices(2);
      indices[0] = 2;
      indices[1] = 0;
      fullprint << "Marginal (2, 0)=" << Distribution(multivariateUserDefined.getMarginal(indices)) << std::endl;

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
