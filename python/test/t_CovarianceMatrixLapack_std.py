#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try:
    matrix1 = CovarianceMatrix(2)
    print "matrix1 (default)=" + repr(matrix1)
    matrix1[0, 0] = 1.0
    matrix1[1, 0] = 0.5
    matrix1[1, 1] = 1.0
    print "matrix1 (initialized)=" + repr(matrix1)

    pt = NumericalPoint()
    pt.add(5.0)
    pt.add(0.0)
    print "pt=" , repr(pt)

    result = NumericalPoint()
    result = matrix1.solveLinearSystem(pt)
    print "result=" + repr(result)

    determinant = matrix1.computeDeterminant()
    print "determinant=%.6f" % determinant

    ev = NumericalScalarCollection(2)
    ev = matrix1.computeEigenValues()
    print "ev=" + repr(ev)

    if matrix1.isPositiveDefinite() :
        isSPD = "true"
    else :
        isSPD = "false"
    print "isSPD=" , isSPD

    matrix2 = matrix1.computeCholesky()
    print "matrix2=" + repr(matrix2)

    b = Matrix(2, 3)
    b[0, 0] =  5.0
    b[1, 0] =  0.0
    b[0, 1] = 10.0
    b[1, 1] =  1.0
    b[0, 2] = 15.0
    b[1, 2] =  2.0
    result2 = matrix1.solveLinearSystem(b, True)
    print "result2=" + repr(result2)

except:
    import sys
    print "t_CovarianceMatrixLapack_std.py", sys.exc_type, sys.exc_value
