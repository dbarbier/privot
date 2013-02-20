#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :

    # External code. This code has an input vector of dimension 4, namely (p0, p1, p2, p3).
    externalCode = NumericalMathFunction( ('E','F','L','I'), ('y',), ('-F*L^3/(3.*E*I)',) )
    dim = externalCode.getInputDimension()

    # The external code will be connected to 2 independent random variables X0 and X1 and one deterministic variable X2 with the following scheme:
#     X2->p0
#     X0->p1
#     X1->p2
#     X0->p3
#     It means that (p0, p1, p2, p3) = A.(X0, X1) + b with:
#     A = [0 0] b = [X2]
#         [1 0]     [ 0]
#         [0 1]     [ 0]
#         [1 0]     [ 0]
#     Here we build the linear function x -> A.x + b

    stochasticDimension = 2
    # UnsignedLong deterministicDimension(1)
    A = Matrix(dim, stochasticDimension)
    A[1, 0] = 1
    A[2, 1] = 1
    A[3, 0] = 1
    b = NumericalPoint(dim, 0)
    X2 = 50.0
    b[0] = X2

    center = NumericalPoint(stochasticDimension , 0.0)

    connect = LinearNumericalMathFunction(center, b, A)
    connect.setName("connect")

    # We are now ready to build the resulting code externalCode(connect()) #
    finalCode = NumericalMathFunction(externalCode, connect)

    # Check if it worked #
    x = NumericalPoint(connect.getInputDimension())
    x[0] = 5
    x[1] = 10
    print "finalCode(x)=", repr(finalCode(x) )
    xRef = NumericalPoint(dim)
    xRef[0] = X2
    xRef[1] = x[0]
    xRef[2] = x[1]
    xRef[3] = x[0]
    print "ref=", repr(externalCode(xRef))

except :
    import sys
    print "t_MergeRandomAndConstantInput.py", sys.exc_type, sys.exc_value
