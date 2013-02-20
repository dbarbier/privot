//                                               -*- C++ -*-
/**
 *  @file  t_StationaryCovarianceModelFactory_std.cxx
 *  @brief The test file of class StationaryCovarianceModelFactory
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
 *  @author: $LastChangedBy: haddad $
 *  @date:   $LastChangedDate: 2011-11-22 12:09:10 +0100 (mar. 22 nov. 2011) $
 *  Id:      $Id: t_StationaryCovarianceModelFactory_std.cxx 2209 2011-11-22 11:09:10Z haddad $
 */
#include <iostream>
#include "OT.hxx"
#include "OTtestcode.hxx"
#include "UserDefinedCovarianceModel.hxx"

using namespace OT;
using namespace OT::Test;


int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  try
    {
      // Dimension of the input model
      // Size of the TimeGrid
      const UnsignedLong size(128);
      const UnsignedLong dimension(1);
      const RegularGrid timeGrid(0., 0.1, size);
      const NumericalPoint amplitude(dimension, 1.0);
      const NumericalPoint scale(dimension, 1.0);
      ExponentialCauchy model(amplitude, scale);
      SpectralNormalProcess myProcess(model, timeGrid);

      // Create a sample of time series
      const UnsignedLong N(1000);
      ProcessSample sample(myProcess.getSample(N));

      // Create the spectral density factory
      WelchFactory spectralFactory;

      // Covariance Factory creation
      StationaryCovarianceModelFactory myFactory(spectralFactory);

      // Build a UserDefinedCovarianceModel using the Wellch method
      UserDefinedStationaryCovarianceModel myCovarianceModel(*myFactory.build(sample));

      // Get the time grid of the model
      RegularGrid myTimeGrid(myCovarianceModel.getTimeGrid());
      for (UnsignedLong i = 0 ; i < myTimeGrid.getN() ; ++i)
        {
          const NumericalScalar t(myTimeGrid.getValue(i));
          const NumericalScalar estimatedValue(myCovarianceModel.computeCovariance(t)(0, 0));
          const NumericalScalar modelValue(model.computeCovariance(t)(0, 0));
          fullprint << "Covariance C(" << t << ") : ";
          fullprint << " evaluation = " << estimatedValue << " model = " << modelValue << std::endl;
        }
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
