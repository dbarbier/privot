#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :

    # Default constructor
    myDefautModel =  UserDefinedStationaryCovarianceModel()
    print "myDefautModel = ", myDefautModel

    # Default dimension parameter to evaluate the model
    dimension = 1

    # Amplitude values
    amplitude = NumericalPoint(dimension)
    # Scale values
    scale = NumericalPoint(dimension)
    # Spatial correclation
    spatialCorrelation = CorrelationMatrix(dimension)
    for index in range(dimension):
        # constant amplitude
        amplitude[index] = 1.0
        scale[index] = (index + 1.0) / dimension

    # Sample a ExponentialModel
    referenceModel = ExponentialModel(amplitude, scale, spatialCorrelation)

    size = 100
    timeGrid = RegularGrid(0.0, 0.1, size)
    covarianceCollection = CovarianceMatrixCollection(size)

    for i in range(timeGrid.getN()):
        t = timeGrid.getValue(i)
        covarianceCollection[i] = referenceModel.computeCovariance(t)
    # Create a UserDefinedStationaryCovarianceModel
    myModel = UserDefinedStationaryCovarianceModel(timeGrid, covarianceCollection)
    print "myModel=", myModel

    for i in range(timeGrid.getN()):
        t = timeGrid.getValue(i)
        # We look for cov(s,t) ==> when adding to the collection, we compute cov(t,s)
        # Because of symmetry, we check the right index computation
        print "myModel = ", myModel.computeCovariance(t)[0, 0], ", referenceModel= ", referenceModel.computeCovariance(t)[0, 0]

except :
    import sys
    print "t_UserDefinedStationaryCovarianceModel_std.py", sys.exc_type, sys.exc_value
