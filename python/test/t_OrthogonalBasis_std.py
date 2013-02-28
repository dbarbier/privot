#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try:
    dim = 3
    polynomCollection = PolynomialFamilyCollection(3)
    polynomCollection[0] = LaguerreFactory(2.5)
    polynomCollection[1] = LegendreFactory()
    polynomCollection[2] = HermiteFactory()
    basisFactory = OrthogonalProductPolynomialFactory(polynomCollection)
    basis = OrthogonalBasis(basisFactory)
    print "Basis=", basis
    point = NumericalPoint(dim, 0.5)
    for i in range(10):
        f = NumericalMathFunction (basis.build(i))
        print "i=", i, " f(point)=", f(point)

except :
    import sys
    print "t_OrthogonalBasis_std.py", sys.exc_type, sys.exc_value
