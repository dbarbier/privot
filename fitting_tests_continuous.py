"""
The test file contains validation and illustrations of
easyfitting module capabilities.
Both the print and get methods of FitContinuousDistribution1D
class are tested.
"""
import openturns as ot
from easyfitting import FitContinuousDistribution1D
import warnings

# Size of sample
size = [10, 100, 1000]

# Distributions to be tested
collection_distribution = ot.DistributionCollection()

# Depending on the OpenTURNS version, some distributions are missing
# Also, in old releases, a cast is mandatory
# We fill the collection of models using a try...except
test_distributions = [ "Arcsine", "Beta", "Burr", "Chi", "ChiSquare",
                       "Dirichlet", "Exponential", "FisherSnedecor",
                       "Gamma", "Gumbel", "InverseNormal", "Laplace",
                       "LogNormal", "LogUniform", "Logistic", "Normal",
                       "Rayleigh", "Rice", "Student", "Trapezoidal",
                       "Triangular", "TruncatedNormal", "Uniform", "Weibull" ]

for str_dist in test_distributions:
    try:
        eval("collection_distribution.add(ot.Distribution(ot." + str_dist + "()))")
    except AttributeError:
        warnings.warn('No ' + str_dist + ' distribution with this current version of OpenTURNS')

# Loop over size
# For each size of sample, the fitting test
# is done. The size play a major role, mainly
# with the Kolmogorov pValue.

for model in collection_distribution:
    print ("Distribution model: " + str(model))
    for N in size:
        # sample of size N ==> getNumericalSample is replaced by getSample
        try:
            sample = model.getSample(N)
        except AttributeError:
            sample = model.getNumericalSample(N)
        print ("Size: " + str(N))
        # Fitting tests
        fitting = FitContinuousDistribution1D(sample)
        # Get the best distribution according to the BIC criterion
        best_bic_distribution = fitting.getBestDistribution(0, 'BIC')
        # Get the best distribution according to the KS criterion
        best_ks_distribution = fitting.getBestDistribution(0, 'KS')
        # print results
        print ("\nDistribution tested")
        fitting.printTestedDistribution('BIC')
        print ("\nDistribution accepted")
        fitting.printAcceptedDistribution('BIC')
        print ("\nBest BIC distribution: " + str(best_bic_distribution))
        print ("Best KS distribution: " + str(best_ks_distribution))
        print ("\nNot tested distribution")
        fitting.printExceptedDistribution()

