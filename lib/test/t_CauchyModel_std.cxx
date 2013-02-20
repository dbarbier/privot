//                                               -*- C++ -*-
/**
 *  @file  t_CauchyModel_std.cxx
 *  @brief The test file of class CauchyModel
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
      CauchyModel myDefautModel;
      fullprint << "myDefautModel = " << myDefautModel << std::endl;

      /* Second order model with parameters */
      CauchyModel myModel(amplitude, scale);
      fullprint << "myModel = " << myModel << std::endl;

      const NumericalScalar frequencyValue(1.0);
      fullprint << "spectral density matrix at f = " << frequencyValue << " : " << myModel.computeSpectralDensity(frequencyValue) << std::endl;

      /* Evaluation at time higher to check the decrease of the cauchy values */
      const NumericalScalar frequencyValueHigh(10.0);
      fullprint << "spectral density matrix at f = " << frequencyValueHigh << " : " << myModel.computeSpectralDensity(frequencyValueHigh) << std::endl;

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
      fullprint << "spatialCorrelation=\n" << spatialCorrelation << std::endl;
      fullprint << "spatialCorrelation=\n" << spatialCorrelation.__str__() << std::endl;

      /* checking the cast */
      SpectralModel mySpectralModel(CauchyModel(amplitude, scale, spatialCorrelation));
      fullprint << "mySpectralModel = " << mySpectralModel << std::endl;

      /* Second order model - high dimension */
      CauchyModel myHighModel(amplitude, scale, spatialCorrelation);
      fullprint << "myHighModel = " << myHighModel << std::endl;

      fullprint << "spectral density matrix at f = " << frequencyValue << " : " << myHighModel.computeSpectralDensity(frequencyValue).clean(1.0e-6) << std::endl;
      fullprint << "spectral density matrix at f = " << frequencyValue << " :\n" << myHighModel.computeSpectralDensity(frequencyValue).clean(1.0e-6).__str__() << std::endl;
      fullprint << "spectral density matrix at f = "  << frequencyValueHigh << " : " << myHighModel.computeSpectralDensity(frequencyValueHigh).clean(1.0e-6) << std::endl;
      fullprint << "spectral density matrix at f = "  << frequencyValueHigh << " :\n" << myHighModel.computeSpectralDensity(frequencyValueHigh).clean(1.0e-6).__str__() << std::endl;

      CovarianceMatrix spatialCovariance(highDimension);
      for (UnsignedLong index = 0 ; index < highDimension; ++index)
        {
          // constant amplitude
          scale[index] = (index + 1.0) / defaultDimension ;
          spatialCovariance(index, index) = 1.0;
          if (index > 0) spatialCorrelation(index, index - 1) = 1.0 / index;
        }

      /* constructor based on spatial covariance */
      CauchyModel myHighModelCovariance(scale, spatialCovariance);
      fullprint << "myHighModelCovariance = " << myHighModelCovariance << std::endl;

      fullprint << "spectral density matrix at f = " << frequencyValue << " : " << myHighModelCovariance.computeSpectralDensity(frequencyValue).clean(1.0e-6) << std::endl;
      fullprint << "spectral density matrix at f = " << frequencyValue << " :\n" << myHighModelCovariance.computeSpectralDensity(frequencyValue).clean(1.0e-6).__str__() << std::endl;
      fullprint << "spectral density matrix at f = "  << frequencyValueHigh << " : " << myHighModelCovariance.computeSpectralDensity(frequencyValueHigh).clean(1.0e-6) << std::endl;
      fullprint << "spectral density matrix at f = "  << frequencyValueHigh << " :\n" << myHighModelCovariance.computeSpectralDensity(frequencyValueHigh).clean(1.0e-6).__str__() << std::endl;

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
