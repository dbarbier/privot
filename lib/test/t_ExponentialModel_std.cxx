//                                               -*- C++ -*-
/**
 *  @file  t_ExponentialModel_std.cxx
 *  @brief The test file of class ExponentialModel
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

  try
    {

      /* Default dimension parameter to evaluate the model */
      const UnsignedLong defaultDimension(1);

      /* Amplitude values */
      NumericalPoint amplitude(defaultDimension, 1.0);
      /* Scale values */
      NumericalPoint scale(defaultDimension, 1.0);


      /* Default constructor */
      ExponentialModel myDefautModel;
      fullprint << "myDefautModel = " << myDefautModel << std::endl;

      /* Second order model with parameters */
      ExponentialModel myModel(amplitude, scale);
      fullprint << "myModel = " << myModel << std::endl;

      const NumericalScalar timeValueOne(1.0);
      fullprint << "covariance matrix at t = " << timeValueOne << " : " << myModel.computeCovariance(timeValueOne) << std::endl;
      fullprint << "covariance matrix at t = " << -1.0 * timeValueOne << " : " << myModel.computeCovariance(-1.0 * timeValueOne) << std::endl;

      /* Evaluation at time higher to check the decrease of the exponential values */
      const NumericalScalar timeValueHigh(4.0);
      fullprint << "covariance matrix at t = "  << timeValueHigh << " : " << myModel.computeCovariance(timeValueHigh) << std::endl;

      /* Discretize the process on a small time grid */
      RegularGrid timeGrid(0.0, 1.0 / 3.0, 4);
      fullprint << "discretized covariance over the time grid=" << timeGrid << " is" << std::endl;
      fullprint << myModel.discretizeCovariance(timeGrid) << std::endl;
      /* Default dimension parameter to evaluate the model */
      const UnsignedLong highDimension(3);

      /* Reallocation of adequate sizes*/
      amplitude.resize(highDimension);
      scale.resize(highDimension);

      CorrelationMatrix spatialCorrelation(highDimension);
      for (UnsignedLong index = 0 ; index < highDimension; ++index)
        {
          // constant amplitude
          amplitude[index] = 1.0 ;
          scale[index] = (index + 1.0) / defaultDimension ;
          if (index > 0) spatialCorrelation(index, index - 1) = 1.0 / index;
        }
      fullprint << "spatialCorrelation=" << spatialCorrelation << std::endl;
      /* checking the cast*/
      StationaryCovarianceModel mySecondOrderModel(ExponentialModel(amplitude, scale, spatialCorrelation));
      fullprint << "mySecondOrderModel = " << mySecondOrderModel << std::endl;

      /* Second order model - high dimension */
      ExponentialModel myHighModel(amplitude, scale, spatialCorrelation);
      fullprint << "myHighModel = " << myHighModel << std::endl;

      fullprint << "covariance matrix at t = " << timeValueOne << " : " << myHighModel.computeCovariance(timeValueOne) << std::endl;
      fullprint << "covariance matrix at t = " << -timeValueOne << " : " << myHighModel.computeCovariance(-timeValueOne) << std::endl;
      fullprint << "covariance matrix at t = "  << timeValueHigh << " : " << myHighModel.computeCovariance(timeValueHigh) << std::endl;

      fullprint << "discretized covariance over the time grid=" << timeGrid << " is" << std::endl;
      fullprint << myHighModel.discretizeCovariance(timeGrid) << std::endl;

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
