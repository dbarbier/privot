#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

def cleanPoint(inPoint) :
    dim = inPoint.getDimension()
    for i in range(dim) :
        inPoint[i] = 1.e-6 * round(1.e6 * inPoint[i])
        if (abs(inPoint[i]) < 1.e-6) :
            inPoint[i] = 0.0
    return inPoint

def cleanMatrix(inMatrix) :
    rowDim = inMatrix.getNbRows()
    colDim = inMatrix.getNbColumns()
    for i in range(rowDim) :
        for j in range(colDim) :
            inMatrix[i, j] = 1.e-6 * round(1.e6 * inMatrix[i, j])
            if (abs(inMatrix[i, j]) < 1.e-6) :
                inMatrix[i, j] = 0.0
    return inMatrix

try :

    dim = 2
    transformation = InverseNatafIndependentCopulaHessian(dim)
    print "transformation=", repr(transformation)
    point = NumericalPoint(dim, 0.75)
    print "transformation(", point, ")=", repr(transformation.hessian(point))
    print "input dimension=", transformation.getInputDimension()
    print "output dimension=", transformation.getOutputDimension()

except :
    import sys
    print "t_InverseNatafIndependentCopulaHessian_std.py", sys.exc_type, sys.exc_value
