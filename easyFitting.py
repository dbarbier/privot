import openturns as ot
from functions import *

class Fit_Continuous_1D_Distribution:
    '''
    NAME
      Fit_Continuous_1D_Distribution
    DESCRIPTION
     Fit_Continuous_1D_Distribution allows to perform statistical fitting tests on a numerical sample or an array of dimension 1.
     The objective is to get a parametric estimation of some distribution models.
     For that purpose, a catalog of all continuous distribution factories implemented in the OpenTURNS library is used.
     From the numerical data and for a fixed distribution model, the parameters have to be estimated. This is done thanks to the maximum
     likelihood principle.
     If the estimation is done, two criteria are computed in order to help making decision:
      * The Bayesian Criterion Information (BIC): This value is in fact the corrected likelihood.
        When fitting some distributions, the likelihood may increase by adding parameters, but doing so may result in overfitting.
        The BIC resolves this problem by introducing a penalty term for the number of parameters in the distribution. The likelihood is also
        normalized by the size of data sample.
      * The Kolmogorov pValue is also computed. This value is issued from the Kolmogorov statistical table and the statistical criterion
        is based on the max norm of difference between empirical/theoritical cumulative functions.
     If the estimation could not be done, the considered distribution name is stored as "excepted distribution"
    The computed models are ranked according to one of the criteria.
    '''
    @staticmethod
    def __checkCriterionArg(criterion):
      assert isinstance(criterion, str)
      uppercriterion = criterion.upper()
      if (uppercriterion not in ["BIC", "KS"]):
          raise ValueError('Expected BIC or KS argument')
      return uppercriterion

    def __init__(self, sample, pvalue = 0.05):
        '''
        ARGUMENTS
            sample : NumericalSample sample of dimension 1
                     Numpy 1D-array
            pvalue : float in ]0, 1[, pValue fixed for Kolmogorov statistical test
                     Default pValue is 5%
        EXAMPLE :
            import openturns as ot
            x = ot.Normal().getSample(100)
            fit = Fit_Continuous_1D_Distribution(x, 0.10)
        '''
        assert isinstance(pvalue, float)
        assert pvalue > 0
        assert pvalue < 1
        self.__pvalue = pvalue
        self.__sample = ot.NumericalSample(sample)
        assert self.__sample.getDimension() == 1
        assert self.__sample.getSize() > 1
        # Get the catalog of all continuous and non parametric factories
        ContinuousDistributionOT = GetAllContinuousFactories()
        self.__ContinuousDistributionOTFactory = ContinuousDistributionOT['AllContinuousFactory']
        self.__ContinuousDistributionOTNames =  ContinuousDistributionOT['AllContinuousFactoryName']
        self.__distributionNames = []
        self.__TestedDistribution = {}
        self.__nbTestedDistributions = 0
        self.__nbAcceptedDistributions = 0
        self.__AcceptedDistribution = None
        # Str attributs
        self.__printTestedDistributionBIC = str()
        self.__printTestedDistributionKS = str()
        self.__printAcceptedDistributionBIC = str()
        self.__printAcceptedDistributionKS = str()
        self.__printExceptedDistribution = '\n---------------- NOT TESTED DISTRIBUTIONS  -------------------------------\n'
        # Used for ranking ==> we may also use python dict or numpy
        # The second option requires numpy
        self.__SortedDistributionAccordingToBIC = ot.NumericalSample(0, 2)
        self.__SortedDistributionAccordingToKS = ot.NumericalSample(0, 2)
        self.__run()

    def __run(self):
        '''
        The method is used to start the statistical computation & fitting.
        No need to call the method since it is automatically launched at the init
        Private ==> not available for users
        '''
        # Default numerical precision
        numerical_precision = ot.PlatformInfo.GetNumericalPrecision()
        # Set precision for numerical printing
        printing_numerical_precision = 3
        ot.PlatformInfo.SetNumericalPrecision(printing_numerical_precision)
        nbFactory = len(self.__ContinuousDistributionOTFactory)
        maxLenTestedDist = 0
        maxLenAcceptedDist = 0
        for i in xrange(nbFactory):
             factory = self.__ContinuousDistributionOTFactory[i]
             Name = self.__ContinuousDistributionOTNames[i]
             self.__distributionNames.append( Name.replace('Factory', '') )
             try:
                 distribution = factory.build(self.__sample)
                 BIC = ot.FittingTest.BIC(self.__sample, distribution, distribution.getParametersNumber())
                 pValue = ot.FittingTest.Kolmogorov(self.__sample, distribution).getPValue()
                 accepted = pValue >= self.__pvalue
                 maxLenTestedDist = max(maxLenTestedDist, len(str(distribution)))
                 if accepted:
                     self.__nbAcceptedDistributions += 1
                     maxLenAcceptedDist = max(maxLenAcceptedDist, len(str(distribution)))
                 dict_elem_res = {"Accepted" : accepted, "BIC": BIC, "pValue" : pValue}
                 # Complete the sample of pValues/BIC ranking
                 self.__SortedDistributionAccordingToBIC.add([i, BIC])
                 self.__SortedDistributionAccordingToKS.add([i, pValue])
                 # Complete the dictionary
                 self.__TestedDistribution[distribution.getName()] = [distribution, dict_elem_res]
                 self.__nbTestedDistributions += 1
             except Exception as e :
                 reasonError = e.message.replace('InvalidArgumentException : ', '')
                 self.__printExceptedDistribution += Name.replace('Factory', ' - ')
                 self.__printExceptedDistribution += '' + reasonError
                 self.__printExceptedDistribution += '\n'
        self.__printExceptedDistribution += '--------------------------------------------------------------------------\n'
        # Rank according to BIC/pValues
        self.__SortedDistributionAccordingToBIC = self.__SortedDistributionAccordingToBIC.sortAccordingToAComponent(1)
        self.__SortedDistributionAccordingToKS = self.__SortedDistributionAccordingToKS.sortAccordingToAComponent(1)
        # Creating string values according to the previous ranking
        ws = ' '
        for k in xrange(self.__nbTestedDistributions):
            index = int(self.__SortedDistributionAccordingToKS[self.__nbTestedDistributions - 1 - k, 0])
            key = self.__distributionNames[index]
            distElem = self.__TestedDistribution[key]
            accepted = distElem[1]['Accepted']
            if accepted:
                acceptedStr = 'Accepted'
            else :
                acceptedStr = 'Rejected'
            self.__printTestedDistributionKS += str(distElem[0]) + (maxLenTestedDist - len(str(distElem[0]))) * ws + '\t' + acceptedStr + '\t' + str(round(distElem[1]['pValue'], printing_numerical_precision)) + '\t' + str(round(distElem[1]['BIC'], printing_numerical_precision)) + '\n'
            if distElem[1]['Accepted']:
                self.__printAcceptedDistributionKS += str(distElem[0]) + (maxLenAcceptedDist - len(str(distElem[0]))) * ws + '\t' + str(round(distElem[1]['pValue'], printing_numerical_precision)) + '\t' + str(round(distElem[1]['BIC'], printing_numerical_precision)) + '\n'
             # Ranking according to BIC
            index = int(self.__SortedDistributionAccordingToBIC[k, 0])
            key = self.__distributionNames[index]
            distElem = self.__TestedDistribution[key]
            accepted = distElem[1]['Accepted']
            if accepted:
                acceptedStr = 'Accepted'
            else :
                acceptedStr = 'Rejected'
            self.__printTestedDistributionBIC += str(distElem[0]) + '\t' + acceptedStr + '\t' + str(round(distElem[1]['pValue'], printing_numerical_precision)) + '\t' + str(round(distElem[1]['BIC'], printing_numerical_precision)) + '\n'
            if distElem[1]['Accepted']:
                self.__printAcceptedDistributionBIC += str(distElem[0]) + '\t' + str(round(distElem[1]['pValue'], printing_numerical_precision)) + '\t' + str(round(distElem[1]['BIC'], printing_numerical_precision)) + '\n'
        # Set default precision
        ot.PlatformInfo.SetNumericalPrecision(numerical_precision)

    def getAcceptedDistribution(self, criterion = 'BIC'):
        '''
        INPUTS
            criterion : string value
                        Should be BIC or KS
                        Default argument is BIC

        OUTPUTS
            collection : DistributionCollection

        DESCRIPTION:
            The method returns the list of distributions that have been tested and accepted
            according to the Kolmogorov test.
            The list is ranked according to this criterion (in that case, input parameter argument should be 'KS')
            or to the BIC criterion (input parameter argument should be 'BIC')

        EXAMPLE :
            import openturns as ot
            x = ot.Normal().getSample(100)
            fit = Fit_Continuous_1D_Distribution(x, 0.10)
            # All accepted distributions ranked using BIC values
            acceptedDistribution = f.getAcceptedDistribution('BIC')
            # Equivalent to :
            acceptedDistribution = f.getAcceptedDistribution()
            # All accepted distributions ranked using KS p-values
            acceptedDistribution = f.getAcceptedDistribution('KS')
        '''
        uppercriterion = self.__checkCriterionArg(criterion)
        if (uppercriterion == 'BIC'):
            return self.getBestBICDistribution(range(self.__nbAcceptedDistributions))
        else:
            return self.getBestKSDistribution(range(self.__nbAcceptedDistributions))

    def getBestBICDistribution(self, index):
        '''
        TODO Documentation
        '''
        assert (isinstance(index, int) or isinstance(index, tuple) or isinstance(index, list))
        if isinstance(index, int):
            listIndex = self.__SortedDistributionAccordingToBIC[index, 0]
            keyValue = self.__distributionNames[int(listIndex)]
            return self.__TestedDistribution[keyValue][0]
        else :
            collection = ot.DistributionCollection()
            for point in index:
                ind = self.__SortedDistributionAccordingToBIC[point, 0]
                keyValue = self.__distributionNames[int(ind)]
                collection.add(self.__TestedDistribution[keyValue][0])
            return collection

    def getBestKSDistribution(self, index):
        '''
        TODO Documentation
        '''
        assert (isinstance(index, int) or isinstance(index, tuple) or isinstance(index, list))
        size = self.__nbTestedDistributions - 1
        if isinstance(index, int):
            if index >= size:
                raise ValueError('Only ' + str(size) + ' distributions have been tested')
            listIndex = self.__SortedDistributionAccordingToKS[size - index, 0]
            name = self.__distributionNames[int(listIndex)]
            distReturned = self.__TestedDistribution[name]
            if distReturned[1]["Accepted"] is False:
                ot.Log.Warn('Care! The distribution has not been accepted by the KS test')
            return distReturned[0]
        else :
            if max(index) >= size:
                raise ValueError('Only ' + str(size) + ' distributions have been tested')
            collection = ot.DistributionCollection()
            for point in index:
                ind = self.__SortedDistributionAccordingToKS[size - point, 0]
                name = self.__distributionNames[int(ind)]
                distReturned = self.__TestedDistribution[name]
                if distReturned[1]["Accepted"] is False:
                    ot.Log.Warn('Care! The distribution has not been accepted by the KS test')
                collection.add(distReturned[0])
            return collection

    def getTestedDistribution(self, criterion = 'BIC'):
        '''
        INPUTS
            criterion : string value
                        Should be BIC or KS
                        Default argument is BIC

        OUTPUTS
            collection : DistributionCollection

        DESCRIPTION:
            The method returns the list of distributions that have been tested.
            The returned list (DistributionCollection) is ranked according to the BIC or Kolmogorov criterion
            The argument fixes the way to rank the collection

        EXAMPLE :
            import openturns as ot
            sample = ot.Uniform().getSample(100)
            fit = Fit_Continuous_1D_Distribution(sample)
            # All tested distributions
            testedDistribution = f.getTestedDistribution('BIC')
            # or
            testedDistribution = f.getTestedDistribution()
            # KS p-values as criterion of ranking
            testedDistribution = f.getTestedDistribution('KS')
        '''
        uppercriterion = self.__checkCriterionArg(criterion)
        if (uppercriterion == 'BIC'):
            return self.getBestBICDistribution(range(self.__nbTestedDistributions))
        else:
            return self.getBestKSDistribution(range(self.__nbTestedDistributions))

    def printAccptededDistribution(self, criterion = "BIC") :
        '''
        INPUTS
            criterion : string value
                        Should be BIC or KS
                        Default argument is BIC

        OUTPUTS
            None

        DESCRIPTION:
            The method prints the list of distributions that have been tested and accepted according
            to the Kolmogorov test.
            The print is done using the following scheme:
              Distribution name | Value1 | Value2
            Where :
              Distribution name : Pretty print of the distribution name
              Value1            : pValue of the Kolmogorov test
              Value2            : BIC value

        EXAMPLE :
            import openturns as ot
            sample = ot.Uniform(0, 1).getSample(100)
            fit = Fit_Continuous_1D_Distribution(sample)
            fit.printAccptededDistribution('BIC')
            fit.printAccptededDistribution()
            fit.printAccptededDistribution('KS')
        '''

        uppercriterion = self.__checkCriterionArg(criterion)
        if (uppercriterion == "BIC"):
            print self.__printAcceptedDistributionBIC
        else:
            print self.__printAcceptedDistributionKS

    def printExceptedDistribution(self) :
        ''' The method print the distributions that have not been tested, with the reason error
        '''
        print self.__printExceptedDistribution

    def printTestedDistribution(self, criterion = "BIC"):
        '''
        INPUTS
            criterion : string value
                        Should be BIC or KS
                        Default argument is BIC

        OUTPUTS
            None

        DESCRIPTION:
            The method prints the list of distributions that have been tested
            The print is done using the following scheme:
              Distribution name | Boolean | Value1 | Value2
            Where :
              Distribution name : Pretty print of the distribution name
              Boolean           : Boolean value (Kolmogorov test Accepted / Rejected)
              Value1            : pValue of the Kolmogorov test
              Value2            : BIC value

        EXAMPLE :
            import openturns as ot
            sample = ot.Uniform(0, 1).getSample(100)
            fit = Fit_Continuous_1D_Distribution(sample)
            fit.printTestedDistribution('BIC')
            fit.printTestedDistribution()
            fit.printTestedDistribution('KS')
        '''
        uppercriterion = self.__checkCriterionArg(criterion)
        if (uppercriterion == "BIC"):
            print self.__printTestedDistributionBIC
        else:
            print self.__printTestedDistributionKS
