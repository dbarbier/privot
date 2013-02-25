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
try:
    collection_distribution.add(ot.Distribution(ot.Arcsine()))
except AttributeError:
    warnings.warn('No Arcsine distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.Beta()))
except AttributeError:
    warnings.warn('No Beta distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.Burr()))
except AttributeError:
    warnings.warn('No Burr distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.Chi()))
except AttributeError:
    warnings.warn('No Chi distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.ChiSquare()))
except AttributeError:
    warnings.warn('No ChiSquare distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.Dirichlet()))
except AttributeError:
    warnings.warn('No Dirichlet distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.Exponential()))
except AttributeError:
    warnings.warn('No Exponential distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.FisherSnedecor()))
except AttributeError:
    warnings.warn('No FisherSnedecor distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.Gamma()))
except AttributeError:
    warnings.warn('No Gamma distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.Gumbel()))
except AttributeError:
    warnings.warn('No Gumbel distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.InverseNormal()))
except AttributeError:
    warnings.warn('No InverseNormal distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.Laplace()))
except AttributeError:
    warnings.warn('No Laplace distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.LogNormal()))
except AttributeError:
    warnings.warn('No LogNormal distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.LogUniform()))
except AttributeError:
    warnings.warn('No LogUniform distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.Logistic()))
except AttributeError:
    warnings.warn('No Logistic distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.Normal()))
except AttributeError:
    warnings.warn('No Normal distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.Rayleigh()))
except AttributeError:
    warnings.warn('No Rayleigh distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.Rice()))
except AttributeError:
    warnings.warn('No Rice distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.Student()))
except AttributeError:
    warnings.warn('No Student distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.Trapezoidal()))
except AttributeError:
    warnings.warn('No Trapezoidal distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.Triangular()))
except AttributeError:
    warnings.warn('No Triangular distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.TruncatedNormal()))
except AttributeError:
    warnings.warn('No TruncatedNormal distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.Uniform()))
except AttributeError:
    warnings.warn('No Uniform distribution with this current version of OpenTURNS')

try:
    collection_distribution.add(ot.Distribution(ot.Weibull()))
except AttributeError:
    warnings.warn('No Weibull distribution with this current version of OpenTURNS')

# Loop over size
# For each size of sample, the fitting test
# is done. The size play a major role, mainly
# with the Kolmogorov pValue.

for model in collection_distribution:
    print ("Distribution model : " + str(model))
    for N in size:
        # sample of size N ==> getNumericalSample is replaced by getSample
        try:
            sample = model.getSample(N)
        except AttributeError:
            sample = model.getNumericalSample(N)
        print ("\nSize : " + str(N))

        # Fitting tests
        fitting = FitContinuousDistribution1D(sample)

        # Get the best distribution according to the BIC criterion
        best_bic_distribution = fitting.getBestDistribution(0, 'BIC')

        # Get the best distribution according to the KS criterion
        best_ks_distribution = fitting.getBestDistribution(0, 'KS')

        # print resuls
        fitting.printTestedDistribution('BIC')
        print ("Distribution accepted")
        fitting.printAcceptedDistribution('BIC')
        print ("Best BIC distribution : " + str(best_bic_distribution))
        print ("Best KS distribution : " + str(best_ks_distribution))
        print ("Not tested distribution")
        fitting.printExceptedDistribution()

