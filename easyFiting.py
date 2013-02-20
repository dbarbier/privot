import openturns as ot
from functions import *

class Fit_Continuous_1D_Distribution:
  '''
   Fitting test class
  '''
    def __init__(sample, pvalue = 0.05):
        self.__sample = ot.NumericalSample(sample)
        self.__printExceptedDistribution = str()
        self.__printTestedDistribution = str()
        self.__AcceptedDistribution = []
        self.__printAcceptedDistribution = str()
        self.__SortedDistributionAccordingToBic = []
        self.__SortedDistributionAccordingToKS = []
        self.__TestedDistribution = {}

    def run(self):
        

    def printExceptedDistribution(self) :
        '''affiche et renvoie une liste de chaîne de caractère
        répertorie l'ensemble des lois pour lesquelles 
        il y a eu une exception (ex:  logNormal alors que l'échantillon contient des valeurs négatives)
        '''
        return None

    def printTestedDistribution(self, arg) :
        '''Affiche un tableau :
            "Nom Distribution  Parmetres de la distribution" (pretty print de la dist) | Accepted | p-value | Bic
            (classé par ordre décroissant du critère Arg où Arg = BIC ou KS)
        '''
        return None

    def printAccptededDistribution(self) :
        '''Affiche le tableau des distributions acceptées uniquement'''
        return None

    def getAcceptedDistribution(self, Arg):
        ''' DistributionCollection ==> des distributions acceptées classée selon (Arg=BIC ou Arg=KS)'''
        return None

    def getBestKSDistribution(self, Arg):
        '''
          si Arg est un entier -> renvoie la Argième meilleure dist au sens KS
          si Arg est une liste -> renvoie les Argièmes meilleures distributions dans une collectionDistribution au sens KS
          ATTENTION : Afficher un Warning si la(les) distribution(s) n'ont pas été acceptées au critère KS
        '''
        return None

    def getBestBICDistribution(self, Arg):
        '''
         si Arg est un entier -> renvoie la Argième meilleure dist au sens BIC
         si Arg est une liste -> renvoie les Argièmes meillueres distributions dans une collectionDistribution au sens BIC
         ATTENTION : Afficher un Warning si la(les) distribution(s) n'ont pas été acceptées au critère KS
        '''
        return None
