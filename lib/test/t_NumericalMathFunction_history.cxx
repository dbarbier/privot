//                                               -*- C++ -*-
/**
 *  @file  t_NumericalMathFunction_history.cxx
 *  @brief The test file of class NumericalMathFunction for history mechanism
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
 *  @date   2011-05-24 19:30:41 +0200 (mar. 24 mai 2011)
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
      NumericalMathFunction f("x", "x^2", "y");
      UnsignedLong size(4);
      NumericalSample input(size, 1);
      for (UnsignedLong i = 0; i < size; ++i) input[i][0] = i;
      NumericalSample output(f(input));
      fullprint << "Is history enabled for f? " << (f.isHistoryEnabled() ? "true" : "false") << std::endl;
      fullprint << "input history=" << f.getInputHistory() << std::endl;
      fullprint << "output history=" << f.getOutputHistory() << std::endl;
      f.enableHistory();
      output = f(input);
      fullprint << "Is history enabled for f? " << (f.isHistoryEnabled() ? "true" : "false") << std::endl;
      fullprint << "input history=" << f.getInputHistory() << std::endl;
      fullprint << "output history=" << f.getOutputHistory() << std::endl;
      f.clearHistory();
      fullprint << "Is history enabled for f? " << (f.isHistoryEnabled() ? "true" : "false") << std::endl;
      fullprint << "input history=" << f.getInputHistory() << std::endl;
      fullprint << "output history=" << f.getOutputHistory() << std::endl;
      // Perform the computation twice
      output = f(input);
      output = f(input);
      fullprint << "input history=" << f.getInputHistory() << std::endl;
      fullprint << "output history=" << f.getOutputHistory() << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
