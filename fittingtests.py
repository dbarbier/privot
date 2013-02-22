"""
The test file contains validation and illustrations of
easyfitting module capabilities.
Both the print and get methods of FitContinuousDistribution1D
class are tested.
"""
import openturns as ot
from easyfitting import FitContinuousDistribution1D

# Size of sample
SIZE = [10, 100, 1000, 10000]


# Loop over size
# For each size of sample, the fitting test
# is done. The size play a major role, mainly
# with the Kolmogorov pValue.

for N in SIZE:
    # Uniform model
    model1 = ot.Uniform(20, 22)
    
    # sample of size N ==> use of getNumericalSample ever if deprecated
    SAMPLE = model1.getNumericalSample(N)

    # Fitting tests
    FIT = FitContinuousDistribution1D(SAMPLE)

    # Get the best distribution according to the BIC criterion
    BICDIST = FIT.getBestDistribution(0, 'BIC')

    # Get the best distribution according to the KS criterion
    KSDIST = FIT.getBestDistribution(0, 'KS')

    # print resuls
    print ("\nSize : " + str(N))
    print ("Distribution model : " + str(model1))
    FIT.printTestedDistribution('BIC')
    print ("Distribution accepted")
    FIT.printAcceptedDistribution('BIC')
    print ("Best BIC distribution : " + str(BICDIST))
    print ("Best KS distribution : " + str(KSDIST))
    print ("Not tested distribution")
    FIT.printExceptedDistribution()
    
    # Uniform model
    model2 = ot.Normal(15.0, 3.0)
    # sample of size N ==> use of getNumericalSample ever if deprecated
    SAMPLE = model2.getNumericalSample(N)

    # Fitting tests
    FIT = FitContinuousDistribution1D(SAMPLE)

    # Get the best distribution according to the BIC criterion
    BICDIST = FIT.getBestDistribution(0, 'BIC')

    # Get the best distribution according to the KS criterion
    KSDIST = FIT.getBestDistribution(0, 'KS')

    print ("Distribution model : " + str(model2))
    print ("Distribution tested")
    FIT.printTestedDistribution('BIC')
    print ("Distribution accepted")
    FIT.printAcceptedDistribution('BIC')
    print ("Best BIC distribution : " + str(BICDIST))
    print ("Best KS distribution : " + str(KSDIST))
    print ("Not tested distribution")
    FIT.printExceptedDistribution()
    
    