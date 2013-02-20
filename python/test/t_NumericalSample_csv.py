#! /usr/bin/env python

from openturns import *
from os import getenv

TESTPREAMBLE()

try :
    Log.Show(Log.INFO)
    envPath = getenv("OPENTURNS_NUMERICALSAMPLE_PATH")
    path = ""
    if (envPath != None) :
        path = envPath
        path += "/"
    aSample = NumericalSample().ImportFromCSVFile(path + "sample_good.csv")
    aSample.setName("a good sample")
    print "aSample=" , repr(aSample)

    aSample = NumericalSample().ImportFromCSVFile(path + "sample_good_coma.csv", ",")
    aSample.setName("a good coma separated sample")
    print "aSample=" , repr(aSample)

    aSample = NumericalSample().ImportFromCSVFile(path + "sample_bad.csv")
    aSample.setName("a sample with bad entries")
    print "aSample with bad entries (see log)=" , repr(aSample)

    aSample = NumericalSample().ImportFromCSVFile(path + "sample_missing.csv")
    aSample.setName("a sample with missing entries")
    print "aSample with missing entries (see log)=" , repr(aSample)

except :
    import sys
    print "t_NumericalSample_csv.py", sys.exc_type, sys.exc_value
