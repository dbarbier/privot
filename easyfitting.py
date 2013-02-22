#                                               -*- Python -*-
#
#  @file  easyfitting.py
#  @brief Helps to fit continuous OpenTURNS distributions
#
#  Copyright (C) 2013 IMACS
#
#  This program is free software; you can redistribute it and/or
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  along with this library.  If not, see <http://www.gnu.org/licenses/>.
#

"""
    OpenTURNS easyfitting
    =============================
    easy use of statistical fitting

    Example
    --------
    >>> import openturns as ot
    >>> from easyfitting import FitContinuousDistribution1D
    >>> x = ot.Normal().getSample(100)
    >>> fit = FitContinuousDistribution1D(x, 0.10)
    >>> test = fit.getTestedDistribution()
    >>> accepted = fit.getAcceptedDistribution()

"""

import openturns as ot
from functions import GetAllContinuousFactories

class FitContinuousDistribution1D:
    '''
    NAME
      FitContinuousDistribution1D
    DESCRIPTION
     FitContinuousDistribution1D allows to perform statistical fitting tests on a numerical sample or an array of dimension 1.
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

    def __init__(self, sample, pvalue=0.05):
        '''
        ARGUMENTS
            sample : NumericalSample sample of dimension 1
                     Numpy 1D-array
            pvalue : float in ]0, 1[, pValue fixed for Kolmogorov statistical test
                     Default pValue is 5%
        EXAMPLE :
            import openturns as ot
            x = ot.Normal().getSample(100)
            fit = FitContinuousDistribution1D(x, 0.10)
        '''
        assert isinstance(pvalue, float)
        assert pvalue > 0
        assert pvalue < 1
        self.__pvalue = pvalue
        self.__sample = ot.NumericalSample(sample)
        assert self.__sample.getDimension() == 1
        assert self.__sample.getSize() > 1
        # Get the catalog of all continuous and non parametric factories
        self.__ContinuousDistributionOTFactory = GetAllContinuousFactories()
        self.__distributionNames = []
        self.__testeddistribution = {}
        self.__nbTestedDistributions = 0
        self.__nbAcceptedDistributions = 0
        # Str attributs
        self.__printtesteddistributionbybicranking = str()
        self.__printtesteddistributionbykolmogorovranking = str()
        self.__printAcceptedDistributionBIC = str()
        self.__printAcceptedDistributionKS = str()
        self.__printExceptedDistribution = \
            '\n---------------- '\
            + 'NOT TESTED DISTRIBUTIONS' + \
            '  -------------------------------\n'
        # Used for ranking ==> we may also use python dict or numpy
        # The second option requires numpy
        self.__sorteddistributionbybic = ot.NumericalSample(0, 2)
        self.__sorteddistributionbykolmogorov = ot.NumericalSample(0, 2)
        self.__run()

    def __run(self):
        '''
        The method is used to start the statistical computation & fitting.
        It is automatically called at the init
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
            Name = factory.getImplementation().getClassName()
            self.__distributionNames.append( Name.replace('Factory', '') )
            try:
                distribution = factory.build(self.__sample)
                distribution_print = str(distribution)
                distribution_name = distribution.getName()
                nbparameters = distribution.getParametersNumber()
                BIC = ot.FittingTest.BIC(self.__sample,
                                         distribution,
                                         nbparameters)
                statisticaltest = ot.FittingTest.Kolmogorov(self.__sample,
                                  distribution)
                pValue = statisticaltest.getPValue()
                accepted = pValue >= self.__pvalue
                maxLenTestedDist = max(maxLenTestedDist, len(distribution_print))
                if accepted:
                    self.__nbAcceptedDistributions += 1
                    maxLenAcceptedDist = max(maxLenAcceptedDist,
                                             len(distribution_print))
                dict_elem_res = {"Accepted" : accepted,
                                 "BIC": BIC,
                                 "pValue" : pValue}
                # Complete the sample of pValues/BIC ranking
                self.__sorteddistributionbybic.add([i, BIC])
                self.__sorteddistributionbykolmogorov.add([i, pValue])
                # Complete the dictionary
                self.__testeddistribution[distribution_name] = [distribution,
                                                                dict_elem_res]
                self.__nbTestedDistributions += 1
            except Exception as e :
                reasonError = \
                    e.message.replace('InvalidArgumentException : ', '')
                self.__printExceptedDistribution += \
                    Name.replace('Factory', ' - ')
                self.__printExceptedDistribution += \
                    '' + reasonError
                self.__printExceptedDistribution += \
                    '\n'
        self.__printExceptedDistribution += 74 * '-' + '\n'
        # Rank according to BIC/pValues
        self.__sorteddistributionbybic = \
            self.__sorteddistributionbybic.sortAccordingToAComponent(1)
        self.__sorteddistributionbykolmogorov = \
            self.__sorteddistributionbykolmogorov.sortAccordingToAComponent(1)
        # WhiteSpace ==> Organize the pretty print
        ws = ' '
        size = self.__nbTestedDistributions - 1
        # Creating string values according to the previous ranking
        for k in xrange(self.__nbTestedDistributions):
            index = self.__sorteddistributionbykolmogorov[size - k, 0]
            index = int(index)
            key = self.__distributionNames[index]
            distElem = self.__testeddistribution[key]
            distribution = distElem[0]
            distribution_print = str(distribution)
            datadistribution = distElem[1]
            if datadistribution['Accepted']:
                acceptedstr = 'Accepted'
                self.__printAcceptedDistributionKS += distribution_print +\
                    (maxLenAcceptedDist - len(distribution_print)) * ws +\
                    '\t' + str(round(datadistribution['pValue'], printing_numerical_precision)) +\
                    '\t' + str(round(datadistribution['BIC'], printing_numerical_precision)) + '\n'
            else :
                acceptedstr = 'Rejected'
            self.__printtesteddistributionbykolmogorovranking += \
                distribution_print + \
                (maxLenTestedDist - len(distribution_print)) * ws + \
                '\t' + acceptedstr + '\t' + str(round(datadistribution['pValue'], printing_numerical_precision)) +\
                '\t' + str(round(datadistribution['BIC'], printing_numerical_precision)) + '\n'

             # Ranking according to BIC
            index = int(self.__sorteddistributionbybic[k, 0])
            key = self.__distributionNames[index]
            distElem = self.__testeddistribution[key]
            distribution = distElem[0]
            distribution_print = str(distribution)
            datadistribution = distElem[1]
            if datadistribution['Accepted']:
                acceptedstr = 'Accepted'
                self.__printAcceptedDistributionBIC += distribution_print + \
                    (maxLenAcceptedDist - len(distribution_print)) * ws  +\
                    '\t' + str(round(datadistribution['pValue'], printing_numerical_precision)) +\
                    '\t' + str(round(datadistribution['BIC'], printing_numerical_precision)) + '\n'
            else :
                acceptedstr = 'Rejected'
            self.__printtesteddistributionbybicranking += distribution_print +\
                (maxLenTestedDist - len(distribution_print)) * ws  +\
                '\t' + acceptedstr + '\t' +\
                str(round(datadistribution['pValue'], printing_numerical_precision)) +\
                '\t' + str(round(datadistribution['BIC'], printing_numerical_precision)) + '\n'
        # Set default precision
        ot.PlatformInfo.SetNumericalPrecision(numerical_precision)

    def getAcceptedDistribution(self, criterion='BIC'):
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
            fit = FitContinuousDistribution1D(x, 0.10)
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
        Documentation
        '''
        assert (isinstance(index, int) or isinstance(index, tuple) or isinstance(index, list))
        if isinstance(index, int):
            listIndex = self.__sorteddistributionbybic[index, 0]
            keyValue = self.__distributionNames[int(listIndex)]
            return self.__testeddistribution[keyValue][0]
        else :
            collection = ot.DistributionCollection()
            for point in index:
                ind = self.__sorteddistributionbybic[point, 0]
                keyValue = self.__distributionNames[int(ind)]
                collection.add(self.__testeddistribution[keyValue][0])
            return collection

    def getBestKSDistribution(self, index):
        '''
        Documentation
        '''
        assert (isinstance(index, int) or isinstance(index, tuple) or isinstance(index, list))
        size = self.__nbTestedDistributions - 1
        if isinstance(index, int):
            if index >= size:
                raise ValueError('Only ' + str(size) + ' distributions have been tested')
            listIndex = self.__sorteddistributionbykolmogorov[size - index, 0]
            name = self.__distributionNames[int(listIndex)]
            distReturned = self.__testeddistribution[name]
            if distReturned[1]["Accepted"] is False:
                ot.Log.Warn('Care! The distribution has not been accepted by the KS test')
            return distReturned[0]
        else :
            if max(index) >= size:
                raise ValueError('Only ' + str(size) + ' distributions have been tested')
            collection = ot.DistributionCollection()
            for point in index:
                ind = self.__sorteddistributionbykolmogorov[size - point, 0]
                name = self.__distributionNames[int(ind)]
                distReturned = self.__testeddistribution[name]
                if distReturned[1]["Accepted"] is False:
                    ot.Log.Warn('Care! The distribution has not been accepted by the KS test')
                collection.add(distReturned[0])
            return collection

    def getTestedDistribution(self, criterion='BIC'):
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
            fit = FitContinuousDistribution1D(sample)
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

    def printAcceptedDistribution(self, criterion = "BIC") :
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
            fit = FitContinuousDistribution1D(sample)
            fit.printAcceptedDistribution('BIC')
            fit.printAcceptedDistribution()
            fit.printAcceptedDistribution('KS')
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

    def printTestedDistribution(self, criterion="BIC"):
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
            fit = FitContinuousDistribution1D(sample)
            fit.printTestedDistribution('BIC')
            fit.printTestedDistribution()
            fit.printTestedDistribution('KS')
        '''
        uppercriterion = self.__checkCriterionArg(criterion)
        if (uppercriterion == "BIC"):
            print self.__printtesteddistributionbybicranking
        else:
            print self.__printtesteddistributionbykolmogorovranking
