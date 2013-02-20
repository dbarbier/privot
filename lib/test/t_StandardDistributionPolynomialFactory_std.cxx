//                                               -*- C++ -*-
/**
 *  @file  t_StandardDistributionPolynomialFactory_std.cxx
 *  @brief The test file of class StandardDistributionPolynomialFactory for standard methods
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
      const UnsignedLong iMax(5);
      Collection<Distribution> distributionCollection;
      distributionCollection.add(Laplace(1.0, 0.0));
      distributionCollection.add(Logistic(0.0, 1.0));
      distributionCollection.add(LogNormal(0.0, 1.0, 0.0));
      distributionCollection.add(Normal(0.0, 1.0));
      distributionCollection.add(Rayleigh(1.0));
      distributionCollection.add(Student(22));
      distributionCollection.add(Triangular(-1.0, 0.3, 1.0));
      distributionCollection.add(Uniform(-1.0, 1.0));
      distributionCollection.add(Weibull(1.0, 3.0));
      for (UnsignedLong n = 0; n < distributionCollection.getSize(); ++n)
        {
          const Distribution distribution(distributionCollection[n]);
          const String name(distribution.getImplementation()->getClassName());
          StandardDistributionPolynomialFactory polynomialFactory(distribution);
          fullprint << "polynomialFactory(" << name << "=" << polynomialFactory << std::endl;
          for (UnsignedLong i = 0; i < iMax; ++i)
            fullprint << name << " polynomial(" << i << ")=" << polynomialFactory.build(i).__str__() << std::endl;
          NumericalPoint roots(polynomialFactory.getRoots(iMax - 1));
          fullprint << name << " polynomial(" << iMax - 1 << ") roots=" << roots << std::endl;
          NumericalPoint weights;
          NumericalPoint nodes(polynomialFactory.getNodesAndWeights(iMax - 1, weights));
          fullprint << name << " polynomial(" << iMax - 1 << ") nodes=" << nodes << " and weights=" << weights <<  std::endl;
        }
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }

  return ExitCode::Success;
}
