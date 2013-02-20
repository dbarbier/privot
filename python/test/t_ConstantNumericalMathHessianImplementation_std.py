#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :

    inputDimension = 3
    outputDimension = 2

    # Constant term
    constant = SymmetricTensor(inputDimension, outputDimension)
    constant[0,0,0] = 7.0
    constant[0,0,1] = -7.0
    constant[0,1,0] = 8.0
    constant[0,1,1] = -8.0
    constant[0,2,0] = 9.0
    constant[0,2,1] = -9.0
    constant[1,0,0] = 8.0
    constant[1,0,1] = -8.0
    constant[1,1,0] = 10.0
    constant[1,1,1] = -10.0
    constant[1,2,0] = 11.0
    constant[1,2,1] = -11.0
    constant[2,0,0] = 9.0
    constant[2,0,1] = -9.0
    constant[2,1,0] = 11.0
    constant[2,1,1] = -11.0
    constant[2,2,0] = 12.0
    constant[2,2,1] = -12.0

    myHessian = ConstantNumericalMathHessianImplementation(constant)
    myHessian.setName("constantHessian")
    inPoint = NumericalPoint(inputDimension)
    inPoint[0] = 7.0
    inPoint[1] = 8.0
    inPoint[2] = 9.0
    outTensor = myHessian.hessian( inPoint )
    print  "myHessian=" , repr(myHessian )
    print  myHessian.getName() , "( " , repr(inPoint) , " ) = " , repr(outTensor)

except :
    import sys
    print "t_ConstantNumericalMathHessianImplementation_std.py", sys.exc_type, sys.exc_value
