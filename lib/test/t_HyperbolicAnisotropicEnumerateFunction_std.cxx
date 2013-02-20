//                                               -*- C++ -*-
/**
 *  @file  t_HyperbolicAnisotropicEnumerateFunction_std.cxx
 *  @brief The test file of class HyperbolicAnisotropicEnumerateFunction for standard methods
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
 *  @author dutka
 *  @date   2008-05-21 17:44:02 +0200 (mer, 21 mai 2008)
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

      fullprint << "Default q : " << ResourceMap::GetAsNumericalScalar("HyperbolicAnisotropicEnumerateFunction-DefaultQ") << std::endl << std::endl;

      // first verify consistency with LinearEnumerateFunction
      UnsignedLong size(10);
      UnsignedLong stratas = 3;
      for (UnsignedLong dimension = 1; dimension < 4; ++ dimension)
        {
          HyperbolicAnisotropicEnumerateFunction f( dimension, 1.0 );
          LinearEnumerateFunction g(dimension);
          fullprint << "First " << size << " values for dimension " << dimension << std::endl;
          for (UnsignedLong index = 0; index < size; ++index)
            {
              if (f(index) != g(index))
                throw InternalException(HERE) << "Results are different";
              fullprint << "index=" << index << " " << f(index) << std::endl;
            }
          Indices strataCardinal;
          for ( UnsignedLong index = 0; index < stratas; ++ index )
            {
              strataCardinal.add( f.getStrataCardinal(index) );
            }
          fullprint << "And first " << stratas << " strata cardinals :" << strataCardinal << std::endl << std::endl;
        }

      // values with varying q
      size = 25;
      NumericalPoint qValues;
      qValues.add(0.75);
      qValues.add(0.5);
      qValues.add(0.25);

      for (UnsignedLong dimension = 2; dimension < 5; ++ dimension)
        {
          for (UnsignedLong j = 0; j < qValues.getDimension(); ++ j)
            {
              NumericalScalar q(qValues[j]);
              fullprint << "First " << size << " values dimension=" << dimension << " q=" << q << std::endl;
              HyperbolicAnisotropicEnumerateFunction f( dimension,  q);
              for (UnsignedLong index = 0; index < size; ++index)
                {
                  fullprint << "index=" << index << " " << f(index) << std::endl;
                }
              Indices strataCardinal;
              for ( UnsignedLong index = 0; index < stratas; ++ index )
                {
                  strataCardinal.add( f.getStrataCardinal(index) );
                }
              fullprint << "And first " << stratas << " strata cardinals :" << strataCardinal << std::endl << std::endl;
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
