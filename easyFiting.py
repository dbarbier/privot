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
        '''affiche et renvoie une liste de cha�ne de caract�re
        r�pertorie l'ensemble des lois pour lesquelles 
        il y a eu une exception (ex:  logNormal alors que l'�chantillon contient des valeurs n�gatives)
        '''
        return None

    def printTestedDistribution(self, arg) :
        '''Affiche un tableau :
            "Nom Distribution  Parmetres de la distribution" (pretty print de la dist) | Accepted | p-value | Bic
            (class� par ordre d�croissant du crit�re Arg o� Arg = BIC ou KS)
        '''
        return None

    def printAccptededDistribution(self) :
        '''Affiche le tableau des distributions accept�es uniquement'''
        return None

    def getAcceptedDistribution(self, Arg):
        ''' DistributionCollection ==> des distributions accept�es class�e selon (Arg=BIC ou Arg=KS)'''
        return None

    def getBestKSDistribution(self, Arg):
        '''
          si Arg est un entier -> renvoie la Argi�me meilleure dist au sens KS
          si Arg est une liste -> renvoie les Argi�mes meilleures distributions dans une collectionDistribution au sens KS
          ATTENTION : Afficher un Warning si la(les) distribution(s) n'ont pas �t� accept�es au crit�re KS
        '''
        return None

    def getBestBICDistribution(self, Arg):
        '''
         si Arg est un entier -> renvoie la Argi�me meilleure dist au sens BIC
         si Arg est une liste -> renvoie les Argi�mes meillueres distributions dans une collectionDistribution au sens BIC
         ATTENTION : Afficher un Warning si la(les) distribution(s) n'ont pas �t� accept�es au crit�re KS
        '''
        return None
