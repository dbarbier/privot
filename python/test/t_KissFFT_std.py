#! /usr/bin/env python

from openturns import *
import math

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :

    # for fft, the best implementation is given for N = 2^p
    size = 16

    # collection for test
    collection = NumericalComplexCollection(size)

    # Fill the data with artificial values

    # Create a complex gaussian sample
    for index in range(size):
        realPart = 0.1 * (index + 1.0) / size
        imagPart = 0.3 * (index + 1.0) / size
        collection[index] = realPart + 1j * imagPart

    # Instanciation of FFT class
    myFFT = KissFFT()
    print "myFFT = ", myFFT

    # Initial transformation
    print "collection = ", collection

    # FFT transform
    transformedCollection = NumericalComplexCollection(myFFT.transform(collection))
    print "FFT result = ", transformedCollection

    # Inverse transformation
    inverseTransformedCollection = NumericalComplexCollection(myFFT.inverseTransform(transformedCollection))
    print "FFT back=", inverseTransformedCollection

except :
    import sys
    print "t_KissFFT_std.py", sys.exc_type, sys.exc_value
