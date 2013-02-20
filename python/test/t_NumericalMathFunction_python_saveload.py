#! /usr/bin/env python

import os
from openturns import *

TESTPREAMBLE()

class FUNC(OpenTURNSPythonFunction):

    def __init__(self):
        OpenTURNSPythonFunction.__init__(self, 2, 1)
        self.setInputDescription( ['R','S'] )
        self.setOutputDescription( ['T'] )

    def _exec(self, X):
        return [ X[0] + X[1] ]


F=FUNC()

#Instance creation
myFunc = NumericalMathFunction( F )

st = Study()
fileName = 'PyNMF.xml'
st.setStorageManager( XMLStorageManager( fileName ) )

st.add( "F", myFunc )
st.save()

print 'saved  func=', myFunc

myFunc = NumericalMathFunction()

st = Study()
st.setStorageManager( XMLStorageManager( fileName ) )

st.load()
st.fillObject( "F", myFunc )

print 'loaded func=', myFunc


inPt = NumericalPoint(2, 2.)
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

#os.remove( fileName )
