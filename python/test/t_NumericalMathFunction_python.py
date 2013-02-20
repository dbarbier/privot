#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

class FUNC(OpenTURNSPythonFunction):

    def __init__(self):
        OpenTURNSPythonFunction.__init__(self, 2, 1)
        self.setInputDescription( ['R','S'] )
        self.setOutputDescription( ['T'] )

    def _exec(self, X):
        Y = [ X[0] + X[1] ]
        return Y

F=FUNC()
print F.getInputDimension(), F.getOutputDimension()

print F( (10,5) )

print F( ((10,5), (6,7)) )


try :

    #Instance creation
    myFunc = NumericalMathFunction( F )

    #Copy constructor
    newFunc = NumericalMathFunction(myFunc)

    print "myFunc input dimension=", myFunc.getInputDimension()
    print "myFunc output dimension=", myFunc.getOutputDimension()

    inPt = NumericalPoint(2, 2.)
    print repr(inPt)

    outPt = myFunc ( inPt )
    print repr(outPt)

    outPt = myFunc ( (10., 11.) )
    print repr(outPt)

    inSample = NumericalSample(10, 2)
    for i in range(10):
        inSample[i] = NumericalPoint( (i,i) )
    print repr(inSample)

    outSample = myFunc ( inSample )
    print repr(outSample)

    outSample = myFunc ( ((100.,100.), (101.,101.), (102.,102.)) )
    print repr(outSample)

    # Test cache behavior
    myFunc.enableCache()
    print 'calls =', myFunc.getEvaluationCallsNumber(), 'hits = ', myFunc.getCacheHits()
    outPt = myFunc ( inPt )
    print 'T =', repr(outPt)
    print 'calls =', myFunc.getEvaluationCallsNumber(), 'hits = ', myFunc.getCacheHits()
    outPt = myFunc ( inPt )
    print 'T =', repr(outPt)
    print 'calls =', myFunc.getEvaluationCallsNumber(), 'hits = ', myFunc.getCacheHits()
    outSample = myFunc ( inSample )
    print 'T =', repr(outSample)
    print 'calls =', myFunc.getEvaluationCallsNumber(), 'hits = ', myFunc.getCacheHits()
    outSample = myFunc ( inSample )
    print 'T =', repr(outSample)
    print 'calls =', myFunc.getEvaluationCallsNumber(), 'hits = ', myFunc.getCacheHits()

    myFunc.clearCache()
    outSample = myFunc ( inSample )
    print 'T =', repr(outSample)
    print 'calls =', myFunc.getEvaluationCallsNumber(), 'hits = ', myFunc.getCacheHits()           

    # test PythonFunction
    def a_exec(X):
        Y = [0]
        Y[0] = X[0] + X[1]
        return Y

    def a_exec_sample(Xs):
        Ys = []
        for X in Xs:
            Ys.append([X[0] + X[1]])
        return Ys

    a_sample = ((100.,100.), (101.,101.), (102.,102.))

    print 'exec'
    myFunc = PythonFunction( 2, 1, a_exec )
    outSample = myFunc( a_sample )
    print outSample
    
    print 'exec + exec_sample'
    myFunc = PythonFunction(2, 1, a_exec, a_exec_sample )
    outSample = myFunc( a_sample )
    print outSample

    print 'exec_sample only on a point'
    myFunc = PythonFunction(2, 1, func_sample=a_exec_sample )
    outSample = myFunc( [100.,100.] )
    print outSample

    print 'exec_sample only on a sample'
    myFunc = PythonFunction(2, 1, func_sample=a_exec_sample )
    outSample = myFunc( a_sample )
    print outSample

    print 'no func'
    try:
        myFunc = PythonFunction(2, 1)
        outSample = myFunc( a_sample )
    except:
        # must raise exception
        print 'no function detected : ok.'
    else:
        raise Exception('no function not detected!')


except :
    import sys, traceback
    traceback.print_exc()
