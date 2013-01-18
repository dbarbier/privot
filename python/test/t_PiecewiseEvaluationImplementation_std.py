#! /usr/bin/env python

from openturns import *

size = 4
locations = NumericalPoint(size)
values = NumericalPoint(size)
# Build locations/values with non-increasing locations
for i in range(size):
    locations[i] = ((i - 0.3 * size) * (i + 0.7 * size)) / size
    values[i] = locations[i] * locations[i]

evaluation = PiecewiseEvaluationImplementation(locations, values)
print "evaluation=", evaluation
# Check the values
for i in range(2 * size):
    x = NumericalPoint(1, (-1.0 + (2.0 * i) / size) * size / 2.0)
    print "f(", x[0], ")=", evaluation(x)
