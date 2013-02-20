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
        assert isinstance(pvalue, float)
        self.__pvalue = pvalue
        self.__sample = ot.NumericalSample(sample)
        assert (self.__sample.getDimension() == 1)
        self.__printExceptedDistribution = '\n---------------- NOT TESTED DISTRIBUTIONS  -------------------------------\n'
        self.__printTestedDistribution = str()
        self.__AcceptedDistribution = []
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
                 ot.Log.Info('Testing : ' + Name)
                 distribution = factory.build(self.__sample)
                 ot.Log.Info('End of test')
                 BIC = ot.FittingTest.BIC(self.__sample, distribution)
                 pValue = ot.FittingTest.Kolmogorov(self.__sample, distribution).getPValue()
                 accepted = pValue >= self.__pvalue
                 dict_elem_res = {"Accepted" : accepted, "BIC": BIC, "pValue" : pValue}
                 # Complete the sample of pValues/BIC ranking
                 self.__SortedDistributionAccordingToBIC.add([i, BIC])
                 self.__SortedDistributionAccordingToKS.add([i, pValue])
                 # Complete the dictionary
                 self.__TestedDistribution[distribution.getName()] = [distribution, dict_elem_res]
                 self.__printTestedDistribution += str(distribution) + '\t' + str(accepted) + '\t' + str(pValue) + '\t' + str(BIC) + '\n'
             except Exception as e :
                 reasonError = e.message.replace('InvalidArgumentException : ', '')
                 self.__printExceptedDistribution += Name.replace('Factory', ' - ')
                 self.__printExceptedDistribution += '' + reasonError
                 self.__printExceptedDistribution += '\n'
        self.__printExceptedDistribution += '--------------------------------------------------------------------------\n'
        self.__SortedDistributionAccordingToBIC = self.__SortedDistributionAccordingToBIC.sortAccordingToAComponent(1)
        self.__SortedDistributionAccordingToKS = self.__SortedDistributionAccordingToKS.sortAccordingToAComponent(1)

    def printExceptedDistribution(self) :
        ''' The method print the distributions that have not been tested, with the reason error
        '''
        print self.__printExceptedDistribution

    def printTestedDistribution(self, arg):
        '''Print a table of tested distributions :
            "Nom Distribution  Parmetres de la distribution" (pretty print de la dist) | Accepted | p-value | Bic
            (classe par ordre decroissant du critere Arg ou Arg = BIC ou KS)
        '''
        upperArg = self.checkArg(arg)
        print self.__printTestedDistribution

    def printAccptededDistribution(self) :
        '''Affiche le tableau des distributions acceptees uniquement'''
        return self.__printAcceptedDistribution

    def getAcceptedDistribution(self, Arg):
        ''' DistributionCollection ==> des distributions acceptees classee selon (Arg=BIC ou Arg=KS)'''
        return self.__AcceptedDistribution

    def getTestedDistribution(self, Arg):
        ''' DistributionCollection ==> des distributions acceptees classee selon (Arg=BIC ou Arg=KS)'''
        return self.__TestedDistribution

    def getBestKSDistribution(self, Arg):
        '''
          si Arg est un entier -> renvoie la Argieme meilleure dist au sens KS
          si Arg est une liste -> renvoie les Argiemes meilleures distributions dans une collectionDistribution au sens KS
          ATTENTION : Afficher un Warning si la(les) distribution(s) n'ont pas ete acceptees au critere KS
        '''
        return None

    def getBestBICDistribution(self, Arg):
        '''
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
        