#! /usr/bin/env python

from openturns import *
from numpy import *

TESTPREAMBLE()

try :

    # Check tuple / NumericalPoint conversion
    t0 = (0.5 ,1.5)
    p0 = NumericalPoint( t0 )
    print "tuple", t0, "=> NumericalPoint", p0

    t1 = tuple( p0 )
    print "NumericalPoint", p0, "=> tuple", t1

    print "NumericalPoint", p0, "+ tuple", t0, "=> NumericalPoint", p0+t0

    # Check list / NumericalPoint conversion
    l0 = [0.5 ,1.5]
    p0 = NumericalPoint( l0 )
    print "list", l0, "=> NumericalPoint", p0

    l1 = list( p0 )
    print "NumericalPoint", p0, "=> list", l1

    print "NumericalPoint", p0, "+ list", l0, "=> NumericalPoint", p0+l0

    # Check sequence protocol for NumericalPoint
    for x in p0:
        print "value", x
    x0 = p0[0]
    p0[0] = x0
    if x0 not in p0:
        raise ValueError, "NumericalPoint badly implements __contains__()"

    # Check array / NumericalPoint conversion
    a0 = array( (0.5 ,1.5) )
    p0 = NumericalPoint( a0 )
    print "array", a0, "=> NumericalPoint", p0

    a1 = array( p0 )
    print "NumericalPoint", p0, "=> array", a1

    print "NumericalPoint", p0, "+ array", a0, "=> NumericalPoint", p0+a0
    print "array", a0, "+ NumericalPoint", p0, "=> array", a0+p0

    # See ticket #423
    m0 = array([[1,1],[2,2],[3,3]])
    try:
        p0 = NumericalPoint(m0)
        print p0
    except:
        print 'Conversion from 2d-array => NumericalPoint failed (as expected :)'

    # Check tuple / NumericalSample conversion
    t0 = ((1.,2.), (3.,4.))
    s0 = NumericalSample( t0 )
    print "tuple", t0, "=> NumericalSample", s0

    t0 = ([1.,2.], [3.,4.])
    s0 = NumericalSample( t0 )
    print "tuple", t0, "=> NumericalSample", s0

    t1 = tuple( s0 )
    print "NumericalSample", s0, "=> tuple", t1

    # Check list / NumericalSample conversion
    l0 = [[1.,2.], [3.,4.]]
    s0 = NumericalSample( l0 )
    print "list", l0, "=> NumericalSample", s0

    l0 = [(1.,2.), (3.,4.)]
    s0 = NumericalSample( l0 )
    print "list", l0, "=> NumericalSample", s0

    l1 = list( s0 )
    print "NumericalSample", s0, "=> list", l1

    # Check array / NumericalSample conversion
    a0 = array( ((1.,2.), (3.,4.)) )
    s0 = NumericalSample( a0 )
    print "array", a0, "=> NumericalSample", s0

    a1 = array( s0 )
    print "NumericalSample", s0, "=> array", a1


    # Check tuple / NumericalMathFunction interoperability
    F = NumericalMathFunction( ('E','F','L','I'), ('d',), ('-F*L^3/(3.*E*I)',) )

    t0 = (1.,2.,3.,4.)
    print "NumericalPoint", F( t0 ), "= F( tuple", t0, ")"

    # Check list / NumericalMathFunction interoperability
    l0 = [1.,2.,3.,4.]
    print "NumericalPoint", F( l0 ), "= F( list", l0, ")"

    # Check array / NumericalMathFunction interoperability
    a0 = array( (1.,2.,3.,4.) )
    print "NumericalPoint", F( a0 ), "= F( array", a0, ")"
    a1 = array( ((1.,2.,3.,4.), (5.,6.,7.,8.)) )
    print "NumericalSample", F( a1 ), "= F( array", a1, ")"

    # Check Python function / NumericalMathFunction interoperability
    def aFunc( x ):
        return [x[0] + x[1] + x[2] + x[3]]

    PYNMF = PythonFunction(4,1,aFunc)

    print "NumericalPoint", PYNMF( t0 ), "= PYNMF( tuple", t0, ")"
    print "NumericalPoint", PYNMF( l0 ), "= PYNMF( list",  l0, ")"
    print "NumericalPoint", PYNMF( a0 ), "= PYNMF( array", a0, ")"
    print "NumericalSample", PYNMF( a1 ), "= PYNMF( array", a1, ")"

    # Check 2-d array which nested dim is size=1 / NumericalMathFunction interoperability
    def aFunc2( x ):
        return [ 2.0 * x[0] ]

    PYNMF = PythonFunction(1,1,aFunc2)

    a0 = array( ([1.]) )
    print "NumericalPoint", PYNMF( a0 ), "= PYNMF( array", a0, ")"
    a1 = array( ([1.], [2.], [3.]) )
    print "NumericalSample", PYNMF( a1 ), "= PYNMF( array", a1, ")"

    # Check tuple / Indices conversion
    t0 = (1, 2)
    i0 = Indices( t0 )
    print "tuple", t0, "=> Indices", i0

    t1 = tuple(i0)
    print "Indices", i0, "=> tuple", tuple([int(x) for x in t0])

    # Check list / Indices conversion
    l0 = [3, 4, 5]
    i0 = Indices( l0 )
    print "list", l0, "=> Indices", i0

    l1 = list( i0 )
    print "Indices", i0, "=> list", [int(x) for x in l1]

    # check Indices typemap
    sample = Normal(3).getSample(10)
    print sample.getDescription()
    marginal = sample.getMarginal((0,2))
    print marginal.getDescription()
    marginal = sample.getMarginal([0,2])
    print marginal.getDescription()

    # Check tuple / Description conversion
    t0 = ('blob', 'zou')
    i0 = Description( t0 )
    print "tuple", t0, "=> Description", i0

    t1 = tuple(i0)
    print "Description", i0, "=> tuple", t1

    # Check list / Description conversion
    l0 = ['blob', 'zou']
    i0 = Description( l0 )
    print "list", l0, "=> Description", i0

    l1 = list( i0 )
    print "Description", i0, "=> list", l1

    # array / Description conversion
    a0 = array(('x0', 'x1', 'x2'))
    d0 = Description( a0 )
    print "array", a0, "=> Description", d0

    a1 = array( d0 )
    print "Description", d0, "=> array", a1

    # check Description typemap
    sample.setDescription(('x0', 'x1', 'x2'))
    print sample.getDescription()
    sample.setDescription(('y0', 'y1', 'y2'))
    print sample.getDescription()
    sample.setDescription(array(('z0', 'z1', 'z2')))
    print sample.getDescription()

    # Check Matrix tuple constructor
    t0 = (1., 2., 3., 4.)

    m0 = Matrix(2, 2, t0)
    print "tuple", t0, "=> Matrix", m0

    m0 = SquareMatrix(2, t0)
    print "tuple", t0, "=> SquareMatrix", m0

    m0 = SymmetricMatrix(2, t0)
    print "tuple", t0, "=> SymmetricMatrix", m0

    m0 = Tensor(2, 2, 1, t0)
    print "tuple", t0, "=> Tensor", m0

    m0 = SymmetricTensor(2, 1, t0)
    print "tuple", t0, "=> SymmetricTensor", m0

    m0 = CorrelationMatrix(2, t0)
    print "tuple", t0, "=> CorrelationMatrix", m0

    m0 = CovarianceMatrix(2, t0)
    print "tuple", t0, "=> CovarianceMatrix", m0

    t0c = (1.+3.j, 2.-5.j, 3.+7.j, 4.-9.j)

    m0 = ComplexMatrix(2, 2, t0)
    print "tuple", t0, "=> ComplexMatrix", m0

    m0 = ComplexMatrix(2, 2, t0c)
    print "tuple", t0c, "=> ComplexMatrix", m0

    m0 = SquareComplexMatrix(2, t0)
    print "tuple", t0, "=> SquareComplexMatrix", m0

    m0 = SquareComplexMatrix(2, t0c)
    print "tuple", t0c, "=> SquareComplexMatrix", m0

    # Check Matrix list constructor
    l0 = [1., 2., 3., 4.]
    m0 = Matrix(2, 2, l0)
    print "list", l0, "=> Matrix", m0

    m0 = SquareMatrix(2, l0)
    print "list", l0, "=> SquareMatrix", m0

    m0 = SymmetricMatrix(2, l0)
    print "list", l0, "=> SymmetricMatrix", m0

    m0 = Tensor(2, 2, 1, l0)
    print "list", l0, "=> Tensor", m0

    m0 = SymmetricTensor(2, 1, l0)
    print "list", l0, "=> SymmetricTensor", m0

    m0 = CorrelationMatrix(2, l0)
    print "list", l0, "=> CorrelationMatrix", m0

    m0 = CovarianceMatrix(2, l0)
    print "list", l0, "=> CovarianceMatrix", m0

    l0c = [1.+3.j, 2.-5.j, 3.+7.j, 4.-9.j]

    m0 = ComplexMatrix(2, 2, l0)
    print "list", l0, "=> ComplexMatrix", m0

    m0 = ComplexMatrix(2, 2, l0c)
    print "list", l0c, "=> ComplexMatrix", m0

    m0 = SquareComplexMatrix(2, l0)
    print "list", l0, "=> SquareComplexMatrix", m0

    m0 = SquareComplexMatrix(2, l0c)
    print "list", l0c, "=> SquareComplexMatrix", m0

    # Check Matrix 1-d array constructor
    a0 = array((1., 2., 3., 4.))
    m0 = Matrix(2, 2, a0)
    print "array", a0, "=> Matrix", m0

    m0 = SquareMatrix(2, a0)
    print "array", a0, "=> SquareMatrix", m0

    m0 = SymmetricMatrix(2, a0)
    print "array", a0, "=> SymmetricMatrix", m0

    m0 = Tensor(2, 2, 1, a0)
    print "array", a0, "=> Tensor", m0

    m0 = SymmetricTensor(2, 1, a0)
    print "array", a0, "=> SymmetricTensor", m0

    m0 = CorrelationMatrix(2, a0)
    print "array", a0, "=> CorrelationMatrix", m0

    m0 = CovarianceMatrix(2, a0)
    print "array", a0, "=> CovarianceMatrix", m0

    a0c = array((1.+3j, 2.-5j, 3.-7j, 4.+9j))

    m0 = ComplexMatrix(2, 2, a0)
    print "array", a0, "=> ComplexMatrix", m0

    m0 = ComplexMatrix(2, 2, a0c)
    print "array", a0c, "=> ComplexMatrix", m0

    m0 = SquareComplexMatrix(2, a0)
    print "array", a0, "=> SquareComplexMatrix", m0

    m0 = SquareComplexMatrix(2, a0c)
    print "array", a0c, "=> SquareComplexMatrix", m0

    # check array / Matrix conversion
    a0 = array( ((1., 2.), (3., 4.), (5., 6.)) )
    m0 = Matrix( a0 )
    print "array", a0, "=> Matrix", m0
    a1 = array( m0 )
    print "Matrix", m0, "=> array", a1

    a0 = array( ((1., 2.), (3., 4.)) )
    m0 = SquareMatrix( a0 )
    print "array", a0, "=> SquareMatrix", m0
    a1 = array( m0 )
    print "SquareMatrix", m0, "=> array", a1

    a0 = array( ((1., 2.), (2., 4.)) )
    m0 = SymmetricMatrix( a0 )
    print "array", a0, "=> SymmetricMatrix", m0
    a1 = array( m0 )
    print "SymmetricMatrix", m0, "=> array", a1

    a0 = array( (((1., 2.), (3., 4.), (5., 6.)), ((7., 8.),(9.,10.),(11.,12.)), ((13., 14.),(15.,16.),(17.,18.)), ((19., 20.),(21.,22.),(23.,24.))) )
    m0 = Tensor( a0 )
    print "array", a0, "=> Tensor", m0
    a1 = array( m0 )
    print "Tensor", m0, "=> array", a1

    a0 = array( (((1., 2.), (3., 4.)), ((3., 4.),(7.,8.))) )
    m0 = SymmetricTensor( a0 )
    print "array", a0, "=> SymmetricTensor", m0
    a1 = array( m0 )
    print "SymmetricTensor", m0, "=> array", a1

    a0 = array( ((2., 1.), (1., 2.)) )
    m0 = CovarianceMatrix( a0 )
    print "array", a0, "=> CovarianceMatrix", m0
    a1 = array( m0 )
    print "CovarianceMatrix", m0, "=> array", a1

    a0 = array( ((1., 0.5), (0.5, 1.)) )
    m0 = CorrelationMatrix( a0 )
    print "array", a0, "=> CorrelationMatrix", m0
    a1 = array( m0 )
    print "CorrelationMatrix", m0, "=> array", a1

    a0 = array( ((1.+3j,2.-5j,3.+7j),(4.-9j,5.+11j,6.-13j)) )
    m0 = ComplexMatrix( a0 )
    print "array", a0, "=> ComplexMatrix", m0
    a1 = array( m0 )
    print "ComplexMatrix", m0, "=> array", a1

    a0 = array( ((1.+3j,2.-5j),(3.+7j,4.-9j)) )
    m0 = SquareComplexMatrix( a0 )
    print "array", a0, "=> SquareComplexMatrix", m0
    a1 = array( m0 )
    print "SquareComplexMatrix", m0, "=> array", a1

    a0 = array( ((1.+3j,0.),(3.+7j,4.-9j)) )
    m0 = TriangularComplexMatrix( a0 )
    print "array", a0, "=> TriangularComplexMatrix", m0
    a1 = array( m0 )
    print "TriangularComplexMatrix", m0, "=> array", a1

    a0 = array( ((1.+3j,2.-5j),(2.+5j,4.-9j)) )
    m0 = HermitianMatrix( a0 )
    print "array", a0, "=> HermitianMatrix", m0
    a1 = array( m0 )
    print "HermitianMatrix", m0, "=> array", a1

    # check np.matrix / Matrix conversion
    a0 = matrix( ((1., 2.), (3., 4.), (5., 6.)) )
    m0 = Matrix( a0 )
    print "matrix", a0, "=> Matrix", m0
    a1 = array( m0 )
    print "Matrix", m0, "=> matrix", a1

    a0 = matrix( ((1.+3j,2.-5j,3.+7j),(4.-9j,5.+11j,6.-13j)) )
    m0 = ComplexMatrix( a0 )
    print "matrix", a0, "=> ComplexMatrix", m0
    a1 = array( m0 )
    print "ComplexMatrix", m0, "=> matrix", a1

except :
    import sys, traceback
    traceback.print_exc(file=sys.stdout)
