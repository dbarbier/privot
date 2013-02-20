#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    #Left hand side of the composition
    left = NumericalMathFunction("TestCompositionLeft")

    #Right hand side of the composition
    right = NumericalMathFunction("TestCompositionRight")

    #Compositon of left and right
    composed = NumericalMathFunction(ComposedNumericalMathFunction(left, right))

    print "right=", repr(right)
    print "left=", repr(left)
    print "composed=", repr(composed)

    #Does it worked?
    x = NumericalPoint(right.getInputDimension(), 1.0)
    y = NumericalPoint(right(x))
    z = NumericalPoint(left(y))
    Dy = Matrix(right.gradient(x))
    Dz = Matrix(left.gradient(y))

    print "x=", repr(x), " y=right(x)=", repr(y), " z=left(y)=", repr(z)
    print "left(right(x))=", repr(composed(x))
    print "D(right)(x)=", repr(Dy), " D(left)(y)=", repr(Dz)
    print " prod=", repr(Dy * Dz)
    print "D(left(right(x)))=", repr(composed.gradient(x))
    result = composed.hessian(x)
    print "DD(left(right(x)))="
    for k in range(result.getNbSheets()) :
        for j in range(result.getNbColumns()) :
            for i in range( result.getNbRows()) :
                print "%.6f" % result[i, j, k]
            print ""
        print ""

except :
    import sys
    print "t_NumericalMathFunction_composition.py", sys.exc_type, sys.exc_value
