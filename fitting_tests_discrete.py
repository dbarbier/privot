"""
The test file contains validation and illustrations of
easyfitting module capabilities.
Both the print and get methods of FitContinuousDistribution1D
class are tested.
"""
import openturns as ot
from easyfitting import FitDiscreteDistribution1D
import warnings

# Size of sample
size = [10, 100, 1000]

# Distributions to be tested
collection_distribution = ot.DistributionCollection()

# Depending on the OpenTURNS version, some distributions are missing
# Also, in old releases, a cast is mandatory
# We fill the collection of models using a try...except
# Care! No default UserDefined
test_distributions = [ "Bernoulli", "Binomial", "Geometric", 
                       "Multinomial", "NegativeBinomial", "Poisson"]


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
        fitting = FitDiscreteDistribution1D(sample)
        # Get the best distribution according to the BIC criterion
        best_bic_distribution = fitting.getBestDistribution(0, 'BIC')
        # Get the best distribution according to the CHI2 criterion
        best_ks_distribution = fitting.getBestDistribution(0, 'CHI2')
        # print results
        fitting.printTestedDistribution('BIC')
        print ("Distribution accepted")
        fitting.printAcceptedDistribution('BIC')
        print ("Best BIC distribution: " + str(best_bic_distribution))
        print ("Best CHI2 distribution: " + str(best_ks_distribution))
        print ("Not tested distribution")
        fitting.printExceptedDistribution()

