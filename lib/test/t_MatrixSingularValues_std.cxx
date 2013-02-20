//                                               -*- C++ -*-
/**
 *  @file  t_MatrixSingularValues_std.cxx
 *  @brief The test file for the singularValues method of the Matrix class
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
 *  @date   2011-05-09 15:48:17 +0200 (lun. 09 mai 2011)
 */
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;

int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);
  {
    // Square case
    Matrix matrix1(3, 3);
    matrix1.setName("matrix1");
    matrix1(0, 0) = 1.0 ;
    matrix1(1, 0) = 2.0 ;
    matrix1(2, 0) = 3.0 ;
    matrix1(0, 1) = 1.0 ;
    matrix1(1, 1) = 4.0 ;
    matrix1(2, 1) = 9.0 ;
    matrix1(0, 2) = 1.0 ;
    matrix1(1, 2) = 8.0 ;
    matrix1(2, 2) = 27.0 ;
    fullprint << "matrix1 = " << matrix1 << std::endl;

    NumericalPoint result1 ;
    result1 = matrix1.computeSingularValues();
    fullprint << "svd (svd only)= " << result1 << std::endl;

    Matrix u;
    Matrix vT;
    result1 = matrix1.computeSingularValues(u, vT, true);
    fullprint << "svd (svd + u, vT full)= " << result1 << std::endl;
    result1 = matrix1.computeSingularValues(u, vT, false);
    fullprint << "svd (svd + u, vT small)= " << result1 << ", u=" << u << ", vT=" << vT << std::endl;
  }
  {
    // Rectangular case, m < n
    Matrix matrix1(3, 5);
    matrix1.setName("matrix1");
    matrix1(0, 0) = 1.0 ;
    matrix1(1, 0) = 2.0 ;
    matrix1(2, 0) = 3.0 ;
    matrix1(0, 1) = 1.0 ;
    matrix1(1, 1) = 4.0 ;
    matrix1(2, 1) = 9.0 ;
    matrix1(0, 2) = 1.0 ;
    matrix1(1, 2) = 8.0 ;
    matrix1(2, 2) = 27.0 ;
    matrix1(0, 3) = 1.0 ;
    matrix1(1, 3) = 16.0 ;
    matrix1(2, 3) = 81.0 ;
    matrix1(0, 4) = 1.0 ;
    matrix1(1, 4) = 32.0 ;
    matrix1(2, 4) = 243.0 ;
    fullprint << "matrix1 = " << matrix1 << std::endl;

    NumericalPoint result1 ;
    result1 = matrix1.computeSingularValues();
    fullprint << "svd (svd only)= " << result1 << std::endl;

    Matrix u;
    Matrix vT;
    result1 = matrix1.computeSingularValues(u, vT, true);
    fullprint << "svd (svd + u, vT full)= " << result1 << std::endl;
    result1 = matrix1.computeSingularValues(u, vT, false);
    fullprint << "svd (svd + u, vT small)= " << result1 << ", u=" << u << ", vT=" << vT << std::endl;
  }
  {
    // Rectangular case, m > n
    Matrix matrix1(5, 3);
    matrix1.setName("matrix1");
    matrix1(0, 0) = 1.0 ;
    matrix1(1, 0) = 2.0 ;
    matrix1(2, 0) = 3.0 ;
    matrix1(3, 0) = 4.0 ;
    matrix1(4, 0) = 5.0 ;
    matrix1(0, 1) = 1.0 ;
    matrix1(1, 1) = 4.0 ;
    matrix1(2, 1) = 9.0 ;
    matrix1(3, 1) = 16.0 ;
    matrix1(4, 1) = 25.0 ;
    matrix1(0, 2) = 1.0 ;
    matrix1(1, 2) = 8.0 ;
    matrix1(2, 2) = 27.0 ;
    matrix1(3, 2) = 64.0 ;
    matrix1(4, 2) = 125.0 ;
    fullprint << "matrix1 = " << matrix1 << std::endl;

    NumericalPoint result1 ;
    result1 = matrix1.computeSingularValues();
    fullprint << "svd (svd only)= " << result1 << std::endl;

    Matrix u;
    Matrix vT;
    result1 = matrix1.computeSingularValues(u, vT, true);
    fullprint << "svd (svd + u, vT full)= " << result1 << std::endl;
    //     result1 = matrix1.computeSingularValues(u, vT, false);
    //     fullprint << "svd (svd + u, vT small)= " << result1 << ", u=" << u << ", vT=" << vT << std::endl;
  }

  return ExitCode::Success;
}
