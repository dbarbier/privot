import openturns as ot
from functions import *

class Fit_Continuous_1D_Distribution:
    '''
     Fitting test class
    '''
    @staticmethod
    def checkArg(Arg):
      assert isinstance(Arg, str)
      upperArg = Arg.upper()
      if (upperArg not in ["BIC", "KS"]):
          raise ValueError('Expected BIC or KS argument')
      return upperArg

    def __init__(self, sample, pvalue = 0.05):
        self.__pvalue = pvalue
        self.__sample = ot.NumericalSample(sample)
        assert self.__sample.getDimension() == 1
        assert self.__sample.getSize() > 1
        assert isinstance(pvalue, float)
        self.__printExceptedDistribution = '\n---------------- NOT TESTED DISTRIBUTIONS  -------------------------------\n'
        self.__printTestedDistributionBIC = str()
        self.__printTestedDistributionKS = str()
        self.__printAcceptedDistribution = str()
        self.__SortedDistributionAccordingToBIC = ot.NumericalSample(0, 2)
        self.__SortedDistributionAccordingToKS = ot.NumericalSample(0, 2)
        self.__TestedDistribution = {}
        # Launch the run method
        ContinuousDistributionOT = GetAllContinuousFactories()
        self.__ContinuousDistributionOTFactory = ContinuousDistributionOT['AllContinuousFactory']
        self.__ContinuousDistributionOTNames =  ContinuousDistributionOT['AllContinuousFactoryName']  
        self.__distributionNames = []
        self.__run()

    def __run(self):
        ''' Run method '''
        # Loop over all known distributions
        size = len(self.__ContinuousDistributionOTFactory)
        for i in xrange(size):
             factory = self.__ContinuousDistributionOTFactory[i]
             Name = self.__ContinuousDistributionOTNames[i]
             self.__distributionNames.append( Name.replace('Factory', '') )
             try:
                 distribution = factory.build(self.__sample)
                 BIC = ot.FittingTest.BIC(self.__sample, distribution)
                 pValue = ot.FittingTest.Kolmogorov(self.__sample, distribution).getPValue()
                 accepted = pValue >= self.__pvalue
                 dict_elem_res = {"Accepted" : accepted, "BIC": BIC, "pValue" : pValue}
                 # Complete the sample of pValues/BIC ranking
                 self.__SortedDistributionAccordingToBIC.add([i, BIC])
                 self.__SortedDistributionAccordingToKS.add([i, pValue])
                 # Complete the dictionary
                 self.__TestedDistribution[distribution.getName()] = [distribution, dict_elem_res]
             except Exception as e :
                 reasonError = e.message.replace('InvalidArgumentException : ', '')
                 self.__printExceptedDistribution += Name.replace('Factory', ' - ')
                 self.__printExceptedDistribution += '' + reasonError
                 self.__printExceptedDistribution += '\n'
        self.__printExceptedDistribution += '--------------------------------------------------------------------------\n'
        # Rank according to BIC/pValues
        self.__SortedDistributionAccordingToBIC = self.__SortedDistributionAccordingToBIC.sortAccordingToAComponent(1)
        self.__SortedDistributionAccordingToKS = self.__SortedDistributionAccordingToKS.sortAccordingToAComponent(1)
        #chaine =  str(distribution) + '\t' + str(accepted) + '\t' + str(pValue) + '\t' + str(BIC) + '\n'

    def printExceptedDistribution(self) :
        ''' The method print the distributions that have not been tested, with the reason error
        '''
        print self.__printExceptedDistribution

    def printTestedDistribution(self, arg = "BIC"):
        '''Print a table of tested distributions :
            "Nom Distribution  Parmetres de la distribution" (pretty print de la dist) | Accepted | p-value | Bic
            (classe par ordre decroissant du critere Arg ou Arg = BIC ou KS)
        '''
        upperArg = self.checkArg(arg)
        if (upperArg == "BIC"):
            print self.__printTestedDistributionBIC
        else:
            print self.__printTestedDistributionKS

    def printAccptededDistribution(self) :
        '''
        TODO Documentation
        '''
        return self.__printAcceptedDistribution

    def getAcceptedDistribution(self, Arg):
        ''' DistributionCollection ==> des distributions acceptees classee selon (Arg=BIC ou Arg=KS)'''
        # TODO! Mail vince
        upperArg = self.checkArg(arg)
        return self.__AcceptedDistribution

    def getTestedDistribution(self, Arg):
        ''' DistributionCollection ==> des distributions acceptees classee selon (Arg=BIC ou Arg=KS)'''
        upperArg = self.checkArg(arg)
        return self.__TestedDistribution

    def getBestKSDistribution(self, Arg):
        '''
        TODO Documentation
        '''
        assert (isinstance(Arg, int) or isinstance(Arg, tuple) or isinstance(Arg, list))
        size = self.__SortedDistributionAccordingToKS.getSize() - 1
        if isinstance(Arg, int):
            listIndex = self.__SortedDistributionAccordingToKS[size - Arg, 0]
            name = self.__distributionNames[int(listIndex)]
            distReturned = self.__TestedDistribution[name]
            if distReturned[1]["accepted"] is False:
                ot.Log.Warn('Care! The distribution has not been accepted by the KS test')
            return distReturned[0]
        else :
            collection = ot.DistributionCollection()
            for point in Arg:
                ind = self.__SortedDistributionAccordingToBIC[size - point, 0]
                name = self.__distributionNames[int(ind)]
                distReturned = self.__TestedDistribution[name]
                if distReturned[1]["accepted"] is False:
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
            name = self.__distributionNames[int(listIndex)]
            return self.__TestedDistribution[name][0]
        else :
            collection = ot.DistributionCollection()
            for point in Arg:
                ind = self.__SortedDistributionAccordingToBIC[point, 0]
                name = self.__distributionNames[int(ind)]
                collection.add(self.__TestedDistribution[name][0])
            return collection
