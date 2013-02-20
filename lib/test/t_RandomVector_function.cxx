//                                               -*- C++ -*-
/**
 *  @file  t_RandomVector_function.cxx
 *  @brief The test file of composite RandomVector class
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

      /* We create a NumericalMathFunction */
      NumericalMathFunction myFunction("wrapper");

      /* We create a distribution */
      UnsignedLong dim(myFunction.getInputDimension());
      CorrelationMatrix R(dim);
      for (UnsignedLong i = 0; i < dim; i++)
        {
          R(i, i) = 1.0;
          for (UnsignedLong j = 0; j < i; j++)
            {
              R(i, j) = NumericalScalar(j + 1) / dim;
            }
        }
      NumericalPoint m(dim, 1.0);
      NumericalPoint s(dim, 2.0);
      Normal distribution(m, s, R);
      const Normal & ref_distribution(distribution);
      fullprint << "distribution = " << ref_distribution << std::endl;


      /* We create a distribution-based RandomVector */
      RandomVector X(distribution);
      fullprint << "X=" << X << std::endl;
      fullprint << "is composite? " << X.isComposite() << std::endl;

      /* Check standard methods of class RandomVector */
      fullprint << "X dimension=" << X.getDimension() << std::endl;
      fullprint << "X realization (first )=" << X.getRealization() << std::endl;
      fullprint << "X realization (second)=" << X.getRealization() << std::endl;
      fullprint << "X realization (third )=" << X.getRealization() << std::endl;
      fullprint << "X sample =" << X.getSample(5) << std::endl;





      /* We create a composite RandomVector Y from X and myFunction */
      RandomVector Y(CompositeRandomVector(myFunction, X));
      fullprint << "Y=" << Y << std::endl;
      fullprint << "is composite? " << Y.isComposite() << std::endl;

      /* Check standard methods of class RandomVector */
      fullprint << "Y dimension=" << Y.getDimension() << std::endl;
      fullprint << "Y realization (first )=" << Y.getRealization() << std::endl;
      fullprint << "Y realization (second)=" << Y.getRealization() << std::endl;
      fullprint << "Y realization (third )=" << Y.getRealization() << std::endl;
      fullprint << "Y sample =" << Y.getSample(5) << std::endl;



    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
