#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()

try :
    # TEST NUMBER ZERO : DEFAULT & COPY CONSTRUCTORs AND STRING CONVERTER
    print "test number zero : default & copy constructors and string converter"

    vectR = NumericalPoint(1, 12.0)
    testSample = NumericalSample(1, 1)

    begin = -1.0
    end = 36.92
    testInterval = ConfidenceInterval(begin, end)
    testInterval.setName("testInterval")

    coll = ConfidenceIntervalCollection()
    coll.add(testInterval)

    valColl = NumericalScalarCollection()
    valColl.add(1.0)

    # Constructor from NumericalPoint
    # Y = Somme (ai * Xi), i=0, n
    # linearModel : (ai), i=0, n
    LM = LinearModel(vectR,coll,valColl)
    emptyLM = LinearModel(vectR)

    # String converter #
    print "LM = ", repr(LM)
    print "emptyLM = " , repr(emptyLM)


    #* TEST NUMBER ONE : GET ELEMENTS #
    print "test number one : get elements"

    # coefficients ai
    print "LM.getRegression = " , repr(LM.getRegression())
    # confidence intervals of the ai coefficients
    print "LM.getConfidenceIntervals = " , repr(LM.getConfidenceIntervals())
    # p values of the (n+1) coefficients ai:
    print "LM.getPValues = " , repr(LM.getPValues())


    #* TEST NUMBER TWO : GET PREDICT/RESIDUAL #
    print "test number two : get predicted/residual"
    lmfact = LinearModelFactory()
    size = 20
    oneSample = NumericalSample(size, 1)
    twoSample = NumericalSample(size, 1)
    for i in range(size):
        oneSample[i] = NumericalPoint(1, 7.0 * sin(-3.5 + (6.5 * i) / (size - 1.0)) + 2.0)
        twoSample[i] = NumericalPoint(1, -2.0 * oneSample[i, 0] + 3.0 + 0.05 * sin(oneSample[i, 0]))
    test=lmfact.build(oneSample,twoSample)
    # evaluate the predictions on sample : predict = Somme(aiXi)
    print "LM.getPredicted = " , repr(test.getPredicted(oneSample))
    # get the residuals epsiloni
    print "LM.getResidual = " , repr(test.getResidual(oneSample,twoSample))

except  :
    import sys
    print "t_LinearModel_std.py", sys.exc_type, sys.exc_value
