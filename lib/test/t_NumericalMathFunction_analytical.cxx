//                                               -*- C++ -*-
/**
 *  @file  t_NumericalMathFunction_analytical.cxx
 *  @brief The test file of class NumericalMathFunction for analytical
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
 *  @date   2012-07-16 12:24:33 +0200 (Mon, 16 Jul 2012)
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
      /* Check the elementary functions */
      Collection<String> elementaryFunctions(0);
      elementaryFunctions.add("sin");
      elementaryFunctions.add("cos");
      elementaryFunctions.add("tan");
      elementaryFunctions.add("asin");
      elementaryFunctions.add("acos");
      elementaryFunctions.add("atan");
      elementaryFunctions.add("sinh");
      elementaryFunctions.add("cosh");
      elementaryFunctions.add("tanh");
      elementaryFunctions.add("asinh");
      elementaryFunctions.add("acosh");
      elementaryFunctions.add("atanh");
      elementaryFunctions.add("log2");
      elementaryFunctions.add("log10");
      elementaryFunctions.add("log");
      elementaryFunctions.add("ln");
      elementaryFunctions.add("lngamma");
      elementaryFunctions.add("gamma");
      elementaryFunctions.add("exp");
      elementaryFunctions.add("erf");
      elementaryFunctions.add("erfc");
      elementaryFunctions.add("sqrt");
      elementaryFunctions.add("cbrt");
      elementaryFunctions.add("besselJ0");
      elementaryFunctions.add("besselJ1");
      elementaryFunctions.add("besselY0");
      elementaryFunctions.add("besselY1");
      elementaryFunctions.add("sign");
      elementaryFunctions.add("rint");
      elementaryFunctions.add("abs");
      /* Check the creation of the elementary functions */
      NumericalPoint x(1, 0.4);
      for (UnsignedLong i = 0; i < elementaryFunctions.getSize(); ++i)
        {
          NumericalPoint x(1, 0.4);

          // acosh only defined for 1 <= x <= pi
          if ( elementaryFunctions[i] == String("acosh") )
            {
              x[0] = 1.4;
            }

          NumericalMathFunction f("x", elementaryFunctions[i] + "(x)", "y");
          std::cout << "f=" << f << std::endl;
          std::cout << "f(" << x[0] << ")=" << std::scientific << std::setprecision(4) << f(x)[0] << std::endl;
          try
            {
              NumericalScalar grad_f = f.gradient(x)(0, 0);
              std::cout << "df(" << x[0] << ")=" << std::scientific << std::setprecision(4) << grad_f << std::endl;
            }
          catch(...)
            {
              std::cout << "finite difference" << std::endl;
              f.setGradientImplementation(new CenteredFiniteDifferenceGradient(ResourceMap::GetAsNumericalScalar( "CenteredFiniteDifferenceGradient-DefaultEpsilon" ), f.getEvaluationImplementation()));
              std::cout << "df(" << x[0] << ")=" << std::scientific << std::setprecision(4) << f.gradient(x)(0, 0) << std::endl;
            }
          try
            {
              NumericalScalar hess_f = f.hessian(x)(0, 0, 0);
              std::cout << "d2f(" << x[0] << ")=" << std::scientific << std::setprecision(4) << hess_f << std::endl;
            }
          catch(...)
            {
              f.setHessianImplementation(new CenteredFiniteDifferenceHessian(ResourceMap::GetAsNumericalScalar( "CenteredFiniteDifferenceHessian-DefaultEpsilon" ), f.getEvaluationImplementation()));
              std::cout << "d2f(" << x[0] << ")=" << std::scientific << std::setprecision(4) << f.hessian(x)(0, 0, 0) << std::endl;
            }
        }
      Description inp(2);
      inp[0] = "x0";
      inp[1] = "x1";
      Description out(2);
      out[0] = "y0";
      out[1] = "y1";
      Description form(2);
      form[0] = "x0+x1";
      form[1] = "x0-x1";

      NumericalMathFunction nmf(inp, out, form);
      NumericalMathFunction marginal0(nmf.getMarginal(0));
      NumericalMathFunction marginal1(nmf.getMarginal(1));
      std::cout << "marginal 0=" << marginal0 << std::endl;
      std::cout << "marginal 1=" << marginal1 << std::endl;
    }
  catch (TestFailed & ex)
    {
      std::cerr << ex << std::endl;
      return ExitCode::Error;
    }


  return ExitCode::Success;
}
