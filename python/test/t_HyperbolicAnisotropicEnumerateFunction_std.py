#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try:
    print "Default Q :", ResourceMap.GetAsNumericalScalar("HyperbolicAnisotropicEnumerateFunction-DefaultQ")

    # check weight constructor
    f = HyperbolicAnisotropicEnumerateFunction( (1.,2.,3.,4.,5.), 0.75 )
    print f

    # verify consistency with LinearEnumerateFunction
    size = 10
    for dimension in range(1, 4):
        f = HyperbolicAnisotropicEnumerateFunction( dimension, 1.0 )
        g = LinearEnumerateFunction( dimension )
        print "First", size, " values for dimension", dimension
        for index in range(size):
            if (not f(index) == g(index)):
                raise Exception('spam', f(index), g(index))
            print "index=", index, repr(f(index))

    # now let q vary
    size = 25
    qValues = NumericalPoint()
    qValues.add(0.75)
    qValues.add(0.5)
    qValues.add(0.25)
    stratas = 3
    for dimension in range(2, 5):
        for j in range(1, qValues.getSize()):
            q = qValues[j]
            print "First", size, " values dimension=", dimension, " q=", q
            f = HyperbolicAnisotropicEnumerateFunction( dimension, q )
            for index in range(size):
                print "index=", index, repr(f(index))
            starteCardinal = Indices()
            for index in range(stratas):
                starteCardinal.add( f.getStrataCardinal(index) )
            print 'And first', stratas, 'strata cardinals :', starteCardinal, '\n'

except :
    import sys
    print "t_HyperbolicAnisotropicEnumerateFunction_std.py", sys.exc_type, sys.exc_value
