#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :
    dimension = 2
    epsilon = NumericalPoint( dimension, 1e-4 )
    x = NumericalPoint( dimension, 2.0 )

    step = ConstantStep( epsilon )
    print 'step type=', step.getClassName(), 'step value=', step( x )

    eta = NumericalPoint( dimension, 1.0 )
    step = BlendedStep( epsilon , eta )
    print 'step type=', step.getClassName(), 'step value=', step( x )

except :
    import sys
    print "t_FiniteDifferenceStep_std.py", sys.exc_type, sys.exc_value
