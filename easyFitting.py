import openturns as ot
from functions import *

class Fit_Continuous_1D_Distribution:
    '''
    NAME
      Fit_Continuous_1D_Distribution
    DESCRIPTION
     Fit_Continuous_1D_Distribution allows to perform statistical fitting tests on a numerical sample or an array of dimension 1.
     For that purpose, the class loads the catalog of all continuous distribution factories implemented in the OpenTURNS library.
     Then, using the numerical data, a fit of the most coherant parameter of the considered distribution is done thanks to the maximum 
     likelihood principle or the moment based method.
     If the estimation is done, two important criteria are computed in order to help making decision:
      * The Bayesian Criterion Information (BIC): This value is in fact the corrected likelihood, the 
     correction terme is issued from the sample size and number of parameters of the distribution.
      * The Kolmogorov pValue is also computed. This value is issued from the Kolmogorov statistical table and the statistical criterion
      is based on the max norm of difference between empirical/theoritical cumulative functions
    '''
    @staticmethod
    def __checkCriterionArg(Arg):
      assert isinstance(Arg, str)
      upperArg = Arg.upper()
      if (upperArg not in ["BIC", "KS"]):
          raise ValueError('Expected BIC or KS argument')
      return upperArg

    def __init__(self, sample, pvalue = 0.05):
        assert isinstance(pvalue, float)
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
        ''' Run method '''
        # Loop over all known distributions
        nbFactory = len(self.__ContinuousDistributionOTFactory)
        for i in xrange(nbFactory):
             factory = self.__ContinuousDistributionOTFactory[i]
             Name = self.__ContinuousDistributionOTNames[i]
             self.__distributionNames.append( Name.replace('Factory', '') )
             try:
                 distribution = factory.build(self.__sample)
                 BIC = ot.FittingTest.BIC(self.__sample, distribution, distribution.getParametersNumber())
                 pValue = ot.FittingTest.Kolmogorov(self.__sample, distribution).getPValue()
                 accepted = pValue >= self.__pvalue
                 if accepted:
                     self.__nbAcceptedDistributions += 1
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
        for k in xrange(self.__nbTestedDistributions):
	    index = int(self.__SortedDistributionAccordingToKS[self.__nbTestedDistributions - 1 - k, 0])
	    key = self.__distributionNames[index]
	    distElem = self.__TestedDistribution[key]
	    self.__printTestedDistributionKS += str(distElem[0])+ '\t' + str(distElem[1]['Accepted']) + '\t' + str(distElem[1]['pValue']) + '\t' + str(distElem[1]['BIC']) + '\n'
	    if distElem[1]['Accepted']:
	      self.__printAcceptedDistributionKS += str(distElem[0])+ '\t' + str(distElem[1]['pValue']) + '\t' + str(distElem[1]['BIC']) + '\n'
             # Ranking according to BIC
	    index = int(self.__SortedDistributionAccordingToBIC[k, 0])
	    key = self.__distributionNames[index]
	    distElem = self.__TestedDistribution[key]
	    self.__printTestedDistributionBIC += str(distElem[0])+ '\t' + str(distElem[1]['Accepted']) + '\t' + str(distElem[1]['pValue']) + '\t' + str(distElem[1]['BIC']) + '\n'
	    if distElem[1]['Accepted']:
	      self.__printAcceptedDistributionBIC += str(distElem[0])+ '\t' + str(distElem[1]['pValue']) + '\t' + str(distElem[1]['BIC']) + '\n'

    def printExceptedDistribution(self) :
        ''' The method print the distributions that have not been tested, with the reason error
        '''
        print self.__printExceptedDistribution

    def printTestedDistribution(self, arg = "BIC"):
        '''Print a table of tested distributions :
            "Nom Distribution  Parmetres de la distribution" (pretty print de la dist) | Accepted | p-value | Bic
            (classe par ordre decroissant du critere Arg ou Arg = BIC ou KS)
        '''
        upperArg = self.__checkCriterionArg(arg)
        if (upperArg == "BIC"):
            print self.__printTestedDistributionBIC
        else:
            print self.__printTestedDistributionKS

    def printAccptededDistribution(self, arg = "BIC") :
        '''
        TODO Documentation
        '''
        upperArg = self.__checkCriterionArg(arg)
        if (upperArg == "BIC"):
            print self.__printAcceptedDistributionBIC
        else:
            print self.__printAcceptedDistributionKS

    def getTestedDistribution(self, Arg):
        ''' DistributionCollection ==> des distributions acceptees classee selon (Arg=BIC ou Arg=KS)'''
        upperArg = self.__checkCriterionArg(arg)
        return self.__TestedDistribution

    def getBestKSDistribution(self, Arg):
        '''
        TODO Documentation
        '''
        assert (isinstance(Arg, int) or isinstance(Arg, tuple) or isinstance(Arg, list))
        size = self.__nbTestedDistributions - 1
        if isinstance(Arg, int):
            if Arg >= size:
                raise ValueError('Only ' + str(size) + ' distributions have been tested')
            listIndex = self.__SortedDistributionAccordingToKS[size - Arg, 0]
            name = self.__distributionNames[int(listIndex)]
            distReturned = self.__TestedDistribution[name]
            if distReturned[1]["Accepted"] is False:
                ot.Log.Warn('Care! The distribution has not been accepted by the KS test')
            return distReturned[0]
        else :
            if max(Arg) >= size:
                raise ValueError('Only ' + str(size) + ' distributions have been tested')
            collection = ot.DistributionCollection()
            for point in Arg:
                ind = self.__SortedDistributionAccordingToKS[size - point, 0]
                name = self.__distributionNames[int(ind)]
                distReturned = self.__TestedDistribution[name]
                if distReturned[1]["Accepted"] is False:
                    ot.Log.Warn('Care! The distribution has not been accepted by the KS test')
                collection.add(distReturned[0])
            return collection

    def getBestBICDistribution(self, Arg):
        '''
        TODO Documentation
        '''
        assert (isinstance(Arg, int) or isinstance(Arg, tuple) or isinstance(Arg, list))
        if isinstance(Arg, int):
            listIndex = self.__SortedDistributionAccordingToBIC[Arg, 0]
            keyValue = self.__distributionNames[int(listIndex)]
            return self.__TestedDistribution[keyValue][0]
        else :
            collection = ot.DistributionCollection()
            for point in Arg:
                ind = self.__SortedDistributionAccordingToBIC[point, 0]
                keyValue = self.__distributionNames[int(ind)]
                collection.add(self.__TestedDistribution[keyValue][0])
            return collection


    def getAcceptedDistribution(self, arg):
        ''' DistributionCollection ==> des distributions acceptees classee selon (Arg=BIC ou Arg=KS)'''
        # TODO
        upperArg = self.__checkCriterionArg(arg)
        if (upperArg == 'BIC'):
            return self.getBestBICDistribution(range(self.__nbAcceptedDistributions))
        else:
            return self.getBestKSDistribution(range(self.__nbAcceptedDistributions))
