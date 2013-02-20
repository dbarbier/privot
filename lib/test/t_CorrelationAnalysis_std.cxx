//                                               -*- C++ -*-
/**
 *  @file  t_CorrelationAnalysis_std.cxx
 *  @brief Test file fo the correlation coefficients computation
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
 *  @author lebrun
 *  @date   2008-04-15 16:24:44 +0200 (mar, 15 avr 2008)
 */
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;

typedef Collection<Distribution> DistributionCollection;

int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  try
    {
      UnsignedLong dimension(2);
      UnsignedLong sampleSize(1000);

      // we create an analytical function
      Description input(dimension);
      input[0] = "x0";
      input[1] = "x1";

      Description foutput("f");

      Description formulas("10+3*x0+x1");

      NumericalMathFunction analytical(input, foutput, formulas);

      // we create a collection of uniform distributions over [-Pi; Pi[
      DistributionCollection aCollection;
      for(UnsignedLong i = 0; i < dimension; ++i)
        {
          aCollection.add(Normal());
        }

      // we create one distribution object
      ComposedDistribution aDistribution(aCollection, IndependentCopula(dimension));

      RandomVector randomVector(aDistribution);
      RandomVector composite(analytical, randomVector);
      fullprint << "composite=" << composite << std::endl;

      // we create two input samples for the function
      NumericalSample inputSample(randomVector.getSample(sampleSize));
      NumericalSample outputSample(analytical(inputSample));

      NumericalPoint src(CorrelationAnalysis::SRC(inputSample, outputSample));
      fullprint << "src=" << src << std::endl;

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }

  return ExitCode::Success;
}
