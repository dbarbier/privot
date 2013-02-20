//                                               -*- C++ -*-
/**
 *  @file  t_BasisSequence_std.cxx
 *  @brief The test file of class BasisSequence for standard methods
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
 *  @date   2010-02-04 16:44:49 +0100 (Thu, 04 Feb 2010)
 */
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;

int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);
  const UnsignedLong dimension = 3;
  Description input;
  Description output[dimension];
  for ( UnsignedLong j = 0; j < dimension; ++ j )
    {
      OSS oss;
      oss << "x" << j;
      String in(oss);
      input.add(in);
      output[j].add(in);
    }
  Collection<NumericalMathFunction> coll;
  Indices cumulBasis;
  Indices partialBasis[dimension];
  for ( UnsignedLong j = 0; j < dimension; ++ j )
    {
      NumericalMathFunction ei(input, output[j], output[j]);
      coll.add(ei);
      cumulBasis.add(j);
      partialBasis[j] = cumulBasis;
    }

  Basis masterBasis(coll);
  BasisSequence seq(masterBasis);
  for ( UnsignedLong j = 0; j < dimension; ++ j )
    {
      seq.add(partialBasis[j]);
    }
  fullprint << "sequence = " << seq << std::endl;
  for ( UnsignedLong j = 0; j < dimension; ++ j )
    {
      fullprint << "indices of subBasis[" << j << "] = " << seq[j] << std::endl;
    }

  return ExitCode::Success;
}
