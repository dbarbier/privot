#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try:
    # Square case
    matrix1 = Matrix(3, 3)
    matrix1.setName("matrix1")
    matrix1[0, 0] = 1.0
    matrix1[1, 0] = 2.0
    matrix1[2, 0] = 3.0
    matrix1[0, 1] = 1.0
    matrix1[1, 1] = 4.0
    matrix1[2, 1] = 9.0
    matrix1[0, 2] = 1.0
    matrix1[1, 2] = 8.0
    matrix1[2, 2] = 27.0
    print "matrix1 = ", repr(matrix1)

    result1 = matrix1.computeSingularValues()
    print "svd (svd only)= ", repr(result1)

    u = Matrix()
    v = Matrix()
    result1 = matrix1.computeSingularValues(u, v, True)
    print "svd (svd + U, V full)= ", repr(result1)
    result1 = matrix1.computeSingularValues(u, v, False)
    print "svd (svd + U, V small)= ", repr(result1), ", U=", repr(u), ", v=", repr(v)

    # Rectangular case, m < n
    matrix1 = Matrix(3, 5)
    matrix1.setName("matrix1")
    matrix1[0, 0] = 1.0
    matrix1[1, 0] = 2.0
    matrix1[2, 0] = 3.0
    matrix1[0, 1] = 1.0
    matrix1[1, 1] = 4.0
    matrix1[2, 1] = 9.0
    matrix1[0, 2] = 1.0
    matrix1[1, 2] = 8.0
    matrix1[2, 2] = 27.0
    matrix1[0, 3] = 1.0
    matrix1[1, 3] = 16.0
    matrix1[2, 3] = 81.0
    matrix1[0, 4] = 1.0
    matrix1[1, 4] = 32.0
    matrix1[2, 4] = 243.0
    print "matrix1 = ", repr(matrix1)

    result1 = matrix1.computeSingularValues()
    print "svd (svd only)= ", repr(result1)

    u = Matrix()
    v = Matrix()
    result1 = matrix1.computeSingularValues(u, v, True)
    print "svd (svd + U, V full)= ", repr(result1)
    result1 = matrix1.computeSingularValues(u, v, False)
    print "svd (svd + U, V small)= ", repr(result1), ", U=", repr(u), ", v=", repr(v)

    # Rectangular case, m > n
    matrix1 = Matrix(5, 3)
    matrix1.setName("matrix1")
    matrix1[0, 0] = 1.0
    matrix1[1, 0] = 2.0
    matrix1[2, 0] = 3.0
    matrix1[3, 0] = 4.0
    matrix1[4, 0] = 5.0
    matrix1[0, 1] = 1.0
    matrix1[1, 1] = 4.0
    matrix1[2, 1] = 9.0
    matrix1[3, 1] = 16.0
    matrix1[4, 1] = 25.0
    matrix1[0, 2] = 1.0
    matrix1[1, 2] = 8.0
    matrix1[2, 2] = 27.0
    matrix1[3, 2] = 64.0
    matrix1[4, 2] = 125.0
    print "matrix1 = ", repr(matrix1)

    result1 = matrix1.computeSingularValues()
    print "svd (svd only)= ", repr(result1)

    u = Matrix()
    v = Matrix()
    result1 = matrix1.computeSingularValues(u, v, True)
    print "svd (svd + U, V full)= ", repr(result1)
    #result1 = matrix1.computeSingularValues(u, v, False)
    #print "svd (svd + U, V small)= ", repr(result1), ", U=", repr(u), ", v=", repr(v)

except:
    import sys
    print "t_MatrixSingularValues_std.py", sys.exc_type, sys.exc_value
