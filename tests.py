"""
First test
The matrix is assumed to be diagonal for validation purposes
"""
if __name__ == "__main__":
    import openturns as ot
    import MultivariateRandomMixture as MV
    collection = ot.DistributionCollection([ot.Normal(), ot.Uniform()])
    matrix = ot.Matrix([[4, 0], [0, 1.4]])
    y = [5, 1]
    distribution = MV.PythonMultivariateRandomMixture(collection, matrix, y)
    interval = distribution.getRange()
    mean = distribution.getMean()
    cov = distribution.getCovariance()
    sigma = distribution.getStandardDeviation()
    N = 2000000
    sample = distribution.getSample(N)
    print "range = ", interval
    print "mean = ", mean
    print "cov = ", cov
    print "sigma = ", sigma
    print "sample :"
    print "min = %s\nmax = %s\nmean = %s" %(sample.getMin(),sample.getMax(),sample.computeMean())
