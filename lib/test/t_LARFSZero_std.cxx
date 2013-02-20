//                                               -*- C++ -*-
/**
 *  @file  t_LARFSZero_std.cxx
 *  @brief The test file of class LARFSZero for standard methods
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
 *  MERCHANTABILITY or FITNESS FOR A PARTICULARFSZero PURPOSE.  See the GNU
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

  NumericalSample x;
  NumericalSample y;
  Basis psi;
  LARFSZero factory;

  fullprint << "factory = " << factory << std::endl;

  BasisSequence seq = factory.build( x, y, psi );

  fullprint << "sequence = " << seq << std::endl;

  return ExitCode::Success;
}
