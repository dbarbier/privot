#! /usr/bin/env python

from openturns import *
import os

TESTPREAMBLE()

try:
    fileName = 'myStudy.xml'

    # Create a Study Object
    myStudy = Study()
    myStudy.setStorageManager(XMLStorageManager(fileName))

    # Add a PersistentObject to the Study (here a NumericalPoint)
    p1 = NumericalPoint(3, 0.)
    p1.setName("Good")
    p1[0] = 10.
    p1[1] = 11.
    p1[2] = 12.
    myStudy.add(p1)

    # Add another PersistentObject to the Study (here a NumericalSample)
    s1 = NumericalSample(3, 2)
    s1.setName("mySample")
    p2 = NumericalPoint(2, 0.)
    p2.setName("One")
    p2[0] = 100.
    p2[1] = 200.
    s1[0] = p2
    p3 = NumericalPoint(2, 0.)
    p3.setName("Two")
    p3[0] = 101.
    p3[1] = 201.
    s1[1] = p3
    p4 = NumericalPoint(2, 0.)
    p4.setName("Three")
    p4[0] = 102.
    p4[1] = 202.
    s1[2] = p4
    myStudy.add("mySample", s1)

    # Add a point with a description
    pDesc = NumericalPointWithDescription(p1)
    desc = pDesc.getDescription()
    desc[0] = "x"
    desc[1] = "y"
    desc[2] = "z"
    pDesc.setDescription(desc)
    myStudy.add(pDesc)

    # Add a matrix
    matrix = Matrix(2,3)
    matrix[0,0] = 0
    matrix[0,1] = 1
    matrix[0,2] = 2
    matrix[1,0] = 3
    matrix[1,1] = 4
    matrix[1,2] = 5
    myStudy.add("m", matrix)

    # Create a NumericalPoint that we will try to reinstaciate after reloading
    point = NumericalPoint(2, 1000.)
    point.setName("point")
    myStudy.add("point", point)

    # Create a NearestPointAlgorithm::Result
    nearestPointAlgorithmResult = NearestPointAlgorithmImplementationResult(NumericalPoint(4, 1.0), 15, 1e-10, 2e-10, 3e-10, 4e-10)
    myStudy.add("nearestPointAlgorithmResult", nearestPointAlgorithmResult)

    # Create a Simulation::Result
    simulationResult = SimulationResult(Event(), 0.5, 0.01, 150, 4)
    myStudy.add("simulationResult", simulationResult)

    # Create a Beta distribution
    beta = Beta(3.0, 5.0, -1.0, 4.0)
    myStudy.add("beta", beta)

    # Create an analytical NumericalMathFunction
    input = Description(3)
    input[0] = "a"
    input[1] = "b"
    input[2] = "c"
    output = Description(3)
    output[0] = "squaresum"
    output[1] = "prod"
    output[2] = "complex"
    formulas = Description(output.getSize())
    formulas[0] = "a+b+c"
    formulas[1] = "a-b*c"
    formulas[2] = "(a+2*b^2+3*c^3)/6"
    analytical = NumericalMathFunction(input, output, formulas)
    analytical.setName("analytical")
    myStudy.add("analytical", analytical)

    # Create a QuadraticCumul algorithm
    antecedent = RandomVector(IndependentCopula(analytical.getInputDimension()))
    antecedent.setName("antecedent")
    composite = RandomVector(analytical, antecedent)
    composite.setName("composite")
    quadraticCumul = QuadraticCumul(composite)
    quadraticCumul.setName("quadraticCumul")
    quadraticCumul.getMeanFirstOrder()
    quadraticCumul.getMeanSecondOrder()
    quadraticCumul.getCovariance()

    myStudy.add("quadraticCumul", quadraticCumul)

    # Create a FORMResult
    input2 = Description(2)
    input2[0] = "x"
    input2[1] = "y"
    output2 = Description(1)
    output2[0] = "d"
    formula2 = Description(1)
    formula2[0] = "y^2-x"
    model = NumericalMathFunction(input2, output2, formula2)
    model.setName("sum")
    input3 = RandomVector(Normal(2))
    input3.setName("input")
    output3 = RandomVector(model, input3)
    output3.setName("output")
    event = Event(output3, ComparisonOperator(Greater()), 1.0)
    event.setName("failureEvent")
    designPoint = NumericalPoint(2, 0.0)
    designPoint[0] = 1.0
    formResult = FORMResult(NumericalPoint(2, 1.0), event, False)
    formResult.setName("formResult")
    formResult.getImportanceFactors()
    formResult.getEventProbabilitySensitivity()
    myStudy.add("formResult", formResult)

    # Create a SORMResult
    sormResult = SORMResult(NumericalPoint(2, 1.0), event, False)
    sormResult.setName("sormResult")
    sormResult.getEventProbabilityBreitung()
    sormResult.getEventProbabilityHohenBichler()
    sormResult.getEventProbabilityTvedt()
    sormResult.getGeneralisedReliabilityIndexBreitung()
    sormResult.getGeneralisedReliabilityIndexHohenBichler()
    sormResult.getGeneralisedReliabilityIndexTvedt()
    myStudy.add("sormResult", sormResult)


    #print "Study = " , myStudy

    myStudy.save()





    # Create a new Study Object
    myStudy = Study()
    myStudy.setStorageManager(XMLStorageManager(fileName))

    myStudy.load()
    #print "loaded Study = " , myStudy

    # Create a NumericalPoint from the one stored in the Study
    point = NumericalPoint()
    myStudy.fillObject("point", point )

    print "point = " , repr(point)

    # Create a NumericalSample from the one stored in the Study
    sample = NumericalSample()
    myStudy.fillObject("mySample", sample)

    print "sample = " , repr(sample)

    # Create a Matrix from the one stored in the Study
    matrix = Matrix()
    myStudy.fillObject("m", matrix)

    print "matrix = " , repr(matrix)

    # Create a NearestPointAlgorithm::Result from the one stored in the Study
    nearestPointAlgorithmResult = NearestPointAlgorithmImplementationResult()
    myStudy.fillObject( "nearestPointAlgorithmResult", nearestPointAlgorithmResult )

    print "nearestPointAlgorithmResult = " , nearestPointAlgorithmResult

    # Create a Simulation::Result from the one stored in the Study
    simulationResult = SimulationResult()
    myStudy.fillObject("simulationResult", simulationResult )

    print "simulation result = " , simulationResult

    # Create a Beta distribution from the one stored in the Study
    beta = Beta()
    myStudy.fillObject("beta", beta )

    print "beta = " , beta

    # Create an analytical NumericalMathFunction from the one stored in the Study
    analytical = NumericalMathFunction()
    myStudy.fillObject( "analytical", analytical )

    print "analytical = ", analytical

    # cleanup
    os.remove(fileName)

except :
    import sys, traceback
    traceback.print_exc()
