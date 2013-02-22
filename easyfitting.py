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
        self._sample = ot.NumericalSample(sample)
        assert self._sample.getDimension() == 1
        assert self._sample.getSize() > 1
        # Get the catalog of all continuous and non parametric factories
        self._ContinuousDistributionOTFactory = GetAllContinuousFactories()
        self._distribution_names = []
        self._testeddistribution = {}
        self._nrTestedDistributions = 0
        self._nrAcceptedDistributions = 0
        # Str attributs
        self._print_tested_distribution_by_bic_ranking = str()
        self._print_tested_distribution_by_kolmogorov_ranking = str()
        self._print_accepted_distribution_bic = str()
        self._print_accepted_distribution_kolmogorov = str()
        self._print_excepted_distribution = \
            '\n---------------- '\
            + 'NOT TESTED DISTRIBUTIONS' + \
            '  -------------------------------\n'
        # Used for ranking ==> we may also use python dict or numpy
        # The second option requires numpy
        self._sorted_distribution_by_bic = ot.NumericalSample(0, 2)
        self._sorted_distribution_by_kolmogorov = ot.NumericalSample(0, 2)
        self._run()

    def _run(self):
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
        nbFactory = len(self._ContinuousDistributionOTFactory)
        maxLenTestedDist = 0
        maxLenAcceptedDist = 0
        for i in xrange(nbFactory):
            factory = self._ContinuousDistributionOTFactory[i]
            Name = factory.getImplementation().getClassName()
            self._distribution_names.append( Name.replace('Factory', '') )
            try:
                distribution = factory.build(self._sample)
                distribution_print = str(distribution)
                distribution_name = distribution.getName()
                nbparameters = distribution.getParametersNumber()
                BIC = ot.FittingTest.BIC(self._sample,
                                         distribution,
                                         nbparameters)
                statisticaltest = ot.FittingTest.Kolmogorov(self._sample,
                                  distribution)
                pValue = statisticaltest.getPValue()
                accepted = pValue >= self.__pvalue
                maxLenTestedDist = max(maxLenTestedDist,
                                   len(distribution_print))
                if accepted:
                    self._nrAcceptedDistributions += 1
                    maxLenAcceptedDist = max(maxLenAcceptedDist,
                                             len(distribution_print))
                dict_elem_res = {"Accepted" : accepted,
                                 "BIC": BIC,
                                 "pValue" : pValue}
                # Complete the sample of pValues/BIC ranking
                self._sorted_distribution_by_bic.add([i, BIC])
                self._sorted_distribution_by_kolmogorov.add([i, pValue])
                # Complete the dictionary
                self._testeddistribution[distribution_name] = [distribution,
                                                                dict_elem_res]
                self._nrTestedDistributions += 1
            except Exception as e :
                reasonError = \
                    e.message.replace('InvalidArgumentException : ', '')
                self._print_excepted_distribution += \
                    Name.replace('Factory', ' - ')
                self._print_excepted_distribution += \
                    '' + reasonError
                self._print_excepted_distribution += \
                    '\n'
        self._print_excepted_distribution += 74 * '-' + '\n'
        # Rank according to BIC/pValues
        self._sorted_distribution_by_bic = \
            self._sorted_distribution_by_bic.sortAccordingToAComponent(1)
        self._sorted_distribution_by_kolmogorov = \
            self._sorted_distribution_by_kolmogorov.sortAccordingToAComponent(1)
        # WhiteSpace ==> Organize the pretty print
        ws = ' '
        size = self._nrTestedDistributions - 1
        # Creating string values according to the previous ranking
        for k in xrange(self._nrTestedDistributions):
            index = self._sorted_distribution_by_kolmogorov[size - k, 0]
            index = int(index)
            key = self._distribution_names[index]
            distElem = self._testeddistribution[key]
            distribution = distElem[0]
            distribution_print = str(distribution)
            datadistribution = distElem[1]
            if datadistribution['Accepted']:
                acceptedstr = 'Accepted'
                self._print_accepted_distribution_kolmogorov += distribution_print +\
                    (maxLenAcceptedDist - len(distribution_print)) * ws +\
                    '\t' + str(round(datadistribution['pValue'], printing_numerical_precision)) +\
                    '\t' + str(round(datadistribution['BIC'], printing_numerical_precision)) + '\n'
            else :
                acceptedstr = 'Rejected'
            self._print_tested_distribution_by_kolmogorov_ranking += \
                distribution_print + \
                (maxLenTestedDist - len(distribution_print)) * ws + \
                '\t' + acceptedstr + '\t' + str(round(datadistribution['pValue'], printing_numerical_precision)) +\
                '\t' + str(round(datadistribution['BIC'], printing_numerical_precision)) + '\n'

             # Ranking according to BIC
            index = int(self._sorted_distribution_by_bic[k, 0])
            key = self._distribution_names[index]
            distElem = self._testeddistribution[key]
            distribution = distElem[0]
            distribution_print = str(distribution)
            datadistribution = distElem[1]
            if datadistribution['Accepted']:
                acceptedstr = 'Accepted'
                self._print_accepted_distribution_bic += distribution_print + \
                    (maxLenAcceptedDist - len(distribution_print)) * ws  +\
                    '\t' + str(round(datadistribution['pValue'], printing_numerical_precision)) +\
                    '\t' + str(round(datadistribution['BIC'], printing_numerical_precision)) + '\n'
            else :
                acceptedstr = 'Rejected'
            self._print_tested_distribution_by_bic_ranking += distribution_print +\
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
            return self.getBestDistribution(range(self._nrAcceptedDistributions), 'BIC')
        else:
            return self.getBestDistribution(range(self._nrAcceptedDistributions), 'KS')

    def getBestDistribution(self, index=0, criterion='BIC'):
        '''
        Documentation
        '''
        assert (isinstance(index, int) or isinstance(index, tuple) or isinstance(index, list))
        uppercriterion = self.__checkCriterionArg(criterion)
        size = self._nrTestedDistributions - 1
        if isinstance(index, int):
            if index >= size:
                raise ValueError('Only ' + str(size) + ' distributions have been tested')
            if (uppercriterion == 'BIC'):
                listIndex = self._sorted_distribution_by_bic[index, 0]
            else:
                listIndex = self._sorted_distribution_by_kolmogorov[size - index, 0]
            keyValue = self._distribution_names[int(listIndex)]
            distReturned = self._testeddistribution[keyValue]
            if distReturned[1]["Accepted"] is False:
                ot.Log.Warn('Care! The distribution has been rejected by the KS test')
            return distReturned[0]
        else : # python sequence
            if max(index) >= size:
                raise ValueError('Only ' + str(size) + ' distributions have been tested')
            collection = ot.DistributionCollection()
            for point in index:
                if (uppercriterion == 'BIC'):
                    ind = self._sorted_distribution_by_bic[point, 0]
                else:
                    ind = self._sorted_distribution_by_kolmogorov[size - point, 0]
                name = self._distribution_names[int(ind)]
                distReturned = self._testeddistribution[name]
                if distReturned[1]["Accepted"] is False:
                    ot.Log.Warn('Care! The distribution has been rejected by the KS test')
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
        index = self._nrTestedDistributions
        return self.getBestDistribution(range(index), uppercriterion)

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
            print self._print_accepted_distribution_bic
        else:
            print self._print_accepted_distribution_kolmogorov

    def printExceptedDistribution(self) :
        ''' The method print the distributions that have 
        not been tested, with the reason error
        '''
        print self._print_excepted_distribution

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
            print self._print_tested_distribution_by_bic_ranking
        else:
            print self._print_tested_distribution_by_kolmogorov_ranking
