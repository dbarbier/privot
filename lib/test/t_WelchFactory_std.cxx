//                                               -*- C++ -*-
/**
 *  @file  t_WelchFactory_std.cxx
 *  @brief The test file of class WelchFactory
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
 *  @author haddad
 *  @date   2011-11-22 12:09:10 +0100 (mar. 22 nov. 2011)
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
      // Dimension of the input model
      // Size of the TimeGrid
      UnsignedLong size(64);
      UnsignedLong dimension(1);
      RegularGrid timeGrid(0.0, 0.1, size);
      NumericalPoint amplitude(dimension, 2.0);
      NumericalPoint scale(dimension, 1.0);
      ExponentialCauchy model(amplitude, scale);
      SpectralNormalProcess myProcess(model, timeGrid);

      // Create a Process sample
      UnsignedLong N(100);
      ProcessSample sample(myProcess.getSample(N));

      // Filtering Windows
      WelchFactory myFactory;

      // Build a UserDefinedSpectralModel using the Wellch method
      SpectralModel mySpectralModel(myFactory.build(sample));

      // Get the frequency grid of the model
      RegularGrid myFrequencyGrid(mySpectralModel.getFrequencyGrid());
      for (UnsignedLong i = 0 ; i < dimension ; ++i)
        {
          for (UnsignedLong j = 0 ; j < dimension ; ++j)
            {
              fullprint << "Spectre " << i << "-" << j << std::endl;
              for (UnsignedLong k = 0; k < myFrequencyGrid.getN(); ++k)
                {
                  const NumericalScalar frequency(myFrequencyGrid.getStart() + k * myFrequencyGrid.getStep());
                  const NumericalScalar estimatedValue(std::real(mySpectralModel.computeSpectralDensity(frequency)(i, j)));
                  const NumericalScalar modelValue(std::real(model.computeSpectralDensity(frequency)(i, j)));
                  fullprint << "Frequency =  " <<  frequency ;
                  fullprint << ", evaluation = " << estimatedValue << " model = " << modelValue << std::endl;
                }
            }
        }
      // Create a Time Series
      TimeSeries timeSeries(myProcess.getRealization());

      // Build a UserDefinedSpectralModel using the Wellch method
      SpectralModel mySpectralModel2(myFactory.build(timeSeries));

      // Get the frequency grid of the model
      myFrequencyGrid = mySpectralModel2.getFrequencyGrid();
      for (UnsignedLong i = 0 ; i < dimension ; ++i)
        {
          for (UnsignedLong j = 0 ; j < dimension ; ++j)
            {
              fullprint << "Spectre " << i << "-" << j << std::endl;
              for (UnsignedLong k = 0; k < myFrequencyGrid.getN(); ++k)
                {
                  const NumericalScalar frequency(myFrequencyGrid.getStart() + k * myFrequencyGrid.getStep());
                  const NumericalScalar estimatedValue(std::real(mySpectralModel2.computeSpectralDensity(frequency)(i, j)));
                  const NumericalScalar modelValue(std::real(model.computeSpectralDensity(frequency)(i, j)));
                  fullprint << "Frequency =  " <<  frequency ;
                  fullprint << ", evaluation = " << estimatedValue << " model = " << modelValue << std::endl;
                }
            }
        }
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
