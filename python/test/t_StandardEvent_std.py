#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator().SetSeed(0)

try :

    # We create a numerical math function */
    myFunction = NumericalMathFunction("poutre")

    dim = myFunction.getInputDimension()

    # We create a normal distribution point of dimension dim
    myDistribution = Normal(dim)

    # We create a 'usual' RandomVector from the Distribution
    vect = RandomVector(myDistribution)

    # We create a composite random vector
    output = RandomVector(myFunction, vect)

    # We create an StandardEvent from this RandomVector
    myStandardEvent = StandardEvent(output,ComparisonOperator(Less()), 1.0)
    print "myStandardEvent=" , myStandardEvent

    # We compute one realization of the event
    # E = (Y=f(X), operator, threshold)
    # E as a RandomVector : Y
    print "myStandardEvent (as a RandomVector) realization =" , repr(RandomVector.getRealization(myStandardEvent))

    # E as a Bernoulli
    print "myStandardEvent realization=" , repr(myStandardEvent.getRealization())

    # We compute a sample of the event
    print "myStandardEvent sample=" , repr(myStandardEvent.getSample(10))

    # Build a standard event based on an event

    R = CorrelationMatrix(dim)
    for i in range(dim - 1) :
        R[i + 1, i] = 0.5

    mean = NumericalPoint(dim, 0.0)
    sigma = NumericalPoint(dim, 1.0)
    myDistribution2 = Normal(mean, sigma, R)

    # We create a 'usual' RandomVector from the Distribution
    vect2 = RandomVector(myDistribution2)

    # We create a composite random vector
    output2 = RandomVector(myFunction, vect2)

    # We create an Event from this RandomVector */
    myEvent = Event(output2, ComparisonOperator(Less()), 1.0)

    # Create a StandardEvent based on this Event */
    stdEvent = StandardEvent(myEvent)

    # Check if the StandardEvent is really a StandardEvent */
    # Get a sample from the second antecedent of the standard event */
    size = 2000
    # Check if the failure probabilities are the same */
    print "Failure probability (Event)=%.6f" % myEvent.getSample(size).computeMean()[0]
    print "Failure probability (StandardEvent)=%.6f" % stdEvent.getSample(size).computeMean()[0]

    collection = UserDefinedPairCollection(3, UserDefinedPair(NumericalPoint(dim), 0.0))
    point = NumericalPoint(dim)
    point[0] = 1.0
    point[1] = 0.5
    point[2] = 1.0
    point[3] = 0.5
    collection[0] = UserDefinedPair(point,0.3)
    point[0] = 2.0
    point[1] = 1.0
    point[2] = 2.0
    point[3] = 1.0
    collection[1] = UserDefinedPair(point,0.325)
    point[0] = 3.0
    point[1] = 1.5
    point[2] = 3.0
    point[3] = 1.5
    collection[2] = UserDefinedPair(point,0.375)
    myDistribution3 = UserDefined(collection)

    # We create a 'usual' RandomVector from the Distribution */
    vect3 = RandomVector(myDistribution3)

    # We create a composite random vector */
    output3 = RandomVector(myFunction, vect3)

    # We try to create a StandardEvent from this RandomVector */
    try :
        myStandardEvent3 = StandardEvent(output3, Less(), 1.0)
    except :
        print "Error trying to build myStandardEvent3"

except :
    import sys
    print "t_StandardEvent_std.py", sys.exc_type, sys.exc_value
