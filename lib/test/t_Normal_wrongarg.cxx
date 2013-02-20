//                                               -*- C++ -*-
/**
 *  @file  t_Normal_wrongarg.cxx
 *  @brief The test file of class Normal with wrong arguments
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;

class TestObject : public Normal
{
public:
  explicit TestObject() : Normal(NumericalPoint(1), NumericalPoint(1), CorrelationMatrix(1)) {}
  explicit TestObject(const String & name) : Normal(NumericalPoint(1), NumericalPoint(1), CorrelationMatrix(1))
  {
    setName(name);
  }
  virtual ~TestObject() {}
};


int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  try
    {

      try
        {

          // Instanciate one distribution object
          NumericalPoint meanPoint(1);
          meanPoint[0] = 1.0;
          NumericalPoint sigma(1);
          sigma[0] = 1.0;
          CorrelationMatrix R(1);
          R(0, 0) = 1.0;
          Normal distribution(meanPoint, sigma, R);
          fullprint << "Distribution " << distribution << std::endl;

          // We try to set an erroneous covariance matrix (wrong dimension) into distribution
          CorrelationMatrix newR(2);
          distribution.setCorrelation(newR);

          // Normally, we should never go here
          throw TestFailed("Exception has NOT been thrown or catched !");


        }
      catch (InvalidArgumentException & ex)
        {
          // Nothing to do
        }

    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
