#! /usr/bin/env python

# use non-interactive backend
import matplotlib
matplotlib.use('Agg')

from openturns.viewer import View
import openturns as ot

# Curve
graph = ot.Normal().drawCDF()
width = 640
height = 480
#graph.draw('curve1', width, height, ot.GraphImplementation.PNG)
view = View(graph, plot_kwargs={'color':'blue'})
#view.save('curve1.png', dpi=fdpi)
view.show(block=False)

# Contour
graph = ot.Normal([1,2],[3,5], ot.CorrelationMatrix(2)).drawPDF()
#graph.draw('curve2', width, height, ot.GraphImplementation.PNG)
view = View(graph)
#view.save('curve2.png', dpi=fdpi)
view.show(block=False)

# Histogram tests
normal = ot.Normal(1)
size = 100
sample = normal.getSample(size)
graph = ot.VisualTest.DrawHistogram(sample, 10)
#graph.draw('curve3', width, height, ot.GraphImplementation.PNG)
view = View(graph)
#view.save('curve3.png', dpi=fdpi)
view.show(block=False)

# QQPlot tests
size = 100
normal = ot.Normal(1)
sample = normal.getSample(size)
sample2 = ot.Gamma(3.0, 4.0, 0.0).getSample(size)
graph = ot.VisualTest.DrawQQplot(sample, sample2, 100)
#graph.draw('curve4', width, height, ot.GraphImplementation.PNG)
view = View(graph)
#view.save('curve4.png', dpi=fdpi)
view.show()

# Clouds tests
dimension = (2)
R = ot.CorrelationMatrix(dimension)
R[0, 1] = 0.8
distribution = ot.Normal(ot.NumericalPoint(dimension, 3.0), ot.NumericalPoint(dimension, 2.0), R)
size = 100
sample2D = distribution.getSample(size)
firstSample = ot.NumericalSample(size, 1)
secondSample = ot.NumericalSample(size, 1)
for i in range(size):
    firstSample[i] = ot.NumericalPoint(1, sample2D[i, 0])
    secondSample[i] = ot.NumericalPoint(1, sample2D[i, 1])
graph = ot.VisualTest.DrawClouds(sample2D, ot.Normal(ot.NumericalPoint(dimension, 2.0), ot.NumericalPoint(dimension, 3.0), R).getSample(divmod(size, 2)[0]))
#graph.draw('curve5', width, height, ot.GraphImplementation.PNG)
view = View(graph)
#view.save('curve5.png', dpi=fdpi)
view.show(block=False)

# CobWeb tests
size = 100
inputDimension = 6
inputSample = ot.Normal(inputDimension).getSample(size)
inputVar = ot.Description(inputDimension)
for i in range(inputDimension):
    inputVar[i] = "X" + str(i)
formula = ot.Description(1)
expression = ""
for i in range(inputDimension):
    if i > 0:
        expression += "+"
    expression += "cos(" + str(i + 1) + "*" + inputVar[i] + ")"
formula[0] = expression
outputVar = ot.Description(1)
outputVar[0] = "y"
model = ot.NumericalMathFunction(inputVar, outputVar, formula)
outputSample = model(inputSample)
graph = ot.VisualTest.DrawCobWeb(inputSample, outputSample, 2.5, 3.0, "red", False)
#graph.draw('curve6', width, height, ot.GraphImplementation.PNG)
view = View( graph, legend_kwargs={'loc': 'lower center'} )
#view.save('curve6.png', dpi=fdpi)
view.show(block=False)

# Staircase
distribution = ot.Poisson(10.0)
graph = distribution.drawCDF()
#graph.draw('curve7', width, height, ot.GraphImplementation.PNG)
view = View(graph)
#view.save('curve7.png', dpi=fdpi)
view.show(block=False)

# Pie
graph = ot.SensitivityAnalysis.DrawImportanceFactors([.4,.3,.2,.1],['a0', 'a1', 'a2', 'a3'],'Zou')
#graph.draw('curve8', width, height, ot.GraphImplementation.PNG)
view = View(graph)
#view.save('curve8.png', dpi=fdpi)
view.show()

# Pairs
dim = 5
meanPoint = ot.NumericalPoint(dim, 0.0)
sigma = ot.NumericalPoint(dim, 1.0)
R = ot.CorrelationMatrix(dim)
for i in range(dim):
    meanPoint[i] = (i + 1) * dim
distribution = ot.Normal(meanPoint, sigma, R)
size = 1000
sample = distribution.getSample( size )
graph = ot.Graph("Pairs", " ", " ", True, "topright")
myPairs = ot.Pairs(sample, "Pairs example", sample.getDescription(), "green", "bullet")
graph.add(myPairs)
#graph.draw('curve9', width, height, ot.GraphImplementation.PNG)
view = View(graph)
#view.save('curve9.png', dpi=fdpi)
view.show(block=False)

# Convergence graph curve
aCollection = ot.DistributionCollection()
aCollection.add(ot.LogNormal(300.,30.,0., ot.LogNormal.MUSIGMA))
aCollection.add(ot.Normal(75e3,5e3))
myDistribution = ot.ComposedDistribution(aCollection)
vect = ot.RandomVector(myDistribution)
LimitState = ot.NumericalMathFunction(("R","F"), ("G",), ("R-F/(_pi*100.0)",))
G = ot.RandomVector(LimitState, vect)
myEvent = ot.Event(G, ot.ComparisonOperator(ot.Less()), 0.0)
myAlgo = ot.MonteCarlo(myEvent)
myAlgo.setMaximumCoefficientOfVariation(0.05)
myAlgo.setMaximumOuterSampling(int(1e5))
myAlgo.run()
graph = myAlgo.drawProbabilityConvergence()
view = View(graph)
#view.save('curve10.png', dpi=fdpi)
view.show()
