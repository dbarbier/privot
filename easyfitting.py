#                                               -*- Python -*-
#
#  @file  easyfitting.py
#  @brief Helps to fit continuous OpenTURNS distributions
#
#  Copyright (C) 2013 EADS IW France
#
#  Written by Sofiane Haddad, IMACS
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
    =====================
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


class _TestedDistribution:
    def __init__(self, distribution, class_name, bic, pvalue, status, message):
        self.distribution = distribution
        self.class_name = class_name
        self.bic = bic
        self.pvalue = pvalue
        self.status = status
        self.message = message

    def __str__(self):
        return "distribution=" + str(self.distribution) + \
            " class=" + self.class_name + \
            " status=" + str(self.status) + \
            " pvalue=" + str(self.pvalue) + " " + \
            " bic=" + str(self.bic) + " " + \
            self.message

class FitContinuousDistribution1D:
    """
    Perform statistical fitting tests on a numerical sample or an 1D array.

    The objective is to get a parametric estimation of some distribution
    models.  For that purpose, a catalog of all continuous distribution
    factories implemented in the OpenTURNS library is used.
    From the numerical data and for a fixed distribution model, the
    parameters have to be estimated. This is done thanks to the maximum
    likelihood principle.
    If the estimation is done, two criteria are computed in order to help
    making decision:
     * The Bayesian Information Criterion (BIC): This value is in fact the
       corrected likelihood.
       When fitting some distributions, the likelihood may increase by adding
       parameters, but doing so may result in overfitting.  The BIC resolves
       this problem by introducing a penalty term for the number of parameters
       in the distribution. The likelihood is also normalized by the size of
       data sample.
     * The Kolmogorov pValue is also computed. This value is issued from the
       Kolmogorov statistical table and the statistical criterion is based on
       the max norm of difference between empirical/theoritical cumulative
       functions.

    If the estimation could not be done, the considered distribution name is
    stored as "excepted distribution".
    The computed models are ranked according to one of the criteria.

    """
    @staticmethod
    def __checkCriterionArg(criterion):
        assert isinstance(criterion, str)
        uppercriterion = criterion.upper()
        if (uppercriterion not in ["BIC", "KS"]):
            raise ValueError('Expected BIC or KS argument')
        return uppercriterion

    @staticmethod
    def __GetAllContinuousFactories():
        """
        Return a dictionary with DistributionFactory objects of OT which generate
        continuous distributions
        """
        # Current list includes parametric and non parametric
        try:
            factories = ot.DistributionFactory.GetContinuousUniVariateFactories()
        except AttributeError:
            #  DistributionFactory.GetContinuousUniVariateFactories() does not
            #  exist before OT 1.2.
            #  Emulate it by looping over all distributions
            factories = []
            for elt in dir(ot.dist):
                if elt.endswith('Factory'):
                    factory_name = 'ot.' + elt
                    str_dist = factory_name.replace('Factory', '()')
                    dist = eval(str_dist)
                    if dist.isContinuous():
                        factory = eval(factory_name + '()')
                        #  WARNING: Mimic GetContinuousUniVariateFactories()
                        factories.append(ot.DistributionFactory(factory))

        # Filter out HistogramFactory
        continuous_factories = []
        for factory in factories:
            factory_name = factory.getImplementation().getClassName()
            if (factory_name != 'HistogramFactory'):
                continuous_factories.append(factory)
        # Returns
        return continuous_factories

    def __init__(self, sample, pvalue=0.05):
        """
        Parameters
        ----------
        sample : 1D array-like
            Either a NumericalSample of dimension 1, or a
            Numpy 1D-array
        pvalue : float in ]0, 1[
            pValue fixed for Kolmogorov statistical test.
            Default pValue is 5%.

        Example
        -------
        >>> import openturns as ot
        >>> from easyfitting import FitContinuousDistribution1D
        >>> x = ot.Normal().getSample(100)
        >>> fit = FitContinuousDistribution1D(x, 0.10)

        """
        assert isinstance(pvalue, float)
        assert pvalue > 0
        assert pvalue < 1
        self.__pvalue = pvalue
        self._sample = ot.NumericalSample(sample)
        assert self._sample.getDimension() == 1
        assert self._sample.getSize() > 1
        # Get the catalog of all continuous and non parametric factories
        self._catalog = []
        for factory in self.__GetAllContinuousFactories():
            factory_name = factory.getImplementation().getClassName()
            name = factory_name.replace('Factory', '')
            reason = ''
            pValue = 0
            BIC = 0.0
            try:
                distribution = factory.build(self._sample)
                nbparameters = distribution.getParametersNumber()
                BIC = ot.FittingTest.BIC(self._sample,
                                         distribution,
                                         nbparameters)
                statisticaltest = ot.FittingTest.Kolmogorov(self._sample,
                                  distribution)
                pValue = statisticaltest.getPValue()
                if pValue >= self.__pvalue:
                    accepted = 1
                else:
                    accepted = 0
            except Exception as e:
                accepted = 2
                distribution = 'Illegal ' + name
                reason = e.message.replace('InvalidArgumentException : ', '')
            self._catalog.append(_TestedDistribution(distribution, name, BIC, pValue, accepted, reason))

    def _getSortedCatalog(self, criterion='BIC'):
        uppercriterion = self.__checkCriterionArg(criterion)
        valid_list = filter(lambda k: k.status < 2, self._catalog)
        if uppercriterion == 'BIC':
            sorted_list = sorted(valid_list, reverse=False, key=lambda t: t.bic)
        else:
            sorted_list = sorted(valid_list, reverse=True,  key=lambda t: t.pvalue)
        return sorted_list

    def getAcceptedDistribution(self, criterion='BIC'):
        """
        Return the list of distributions that have been tested and accepted
        according to the Kolmogorov test.

        The list is ranked according to the parameter passed to the constructor.

        Parameters
        ----------
        criterion : string
            Must be either 'BIC' or 'KS'.  Default is 'BIC'.

        Returns
        -------
        out : DistributionCollection

        Example
        -------
        >>> import openturns as ot
        >>> from easyfitting import FitContinuousDistribution1D
        >>> x = ot.Normal().getSample(100)
        >>> fit = FitContinuousDistribution1D(x, 0.10)
        >>> # All accepted distributions ranked using BIC values
        >>> acceptedDistribution = fit.getAcceptedDistribution('BIC')
        >>> # Equivalent to :
        >>> acceptedDistribution = fit.getAcceptedDistribution()
        >>> # All accepted distributions ranked using KS p-values
        >>> acceptedDistribution = fit.getAcceptedDistribution('KS')
        """
        uppercriterion = self.__checkCriterionArg(criterion)
        nrAcceptedDistributions = len(filter(lambda k: k.status < 2, self._catalog))
        return self.getBestDistribution(range(nrAcceptedDistributions), uppercriterion)

    def getBestDistribution(self, index=0, criterion='BIC'):
        '''
        Return a distribution (in case that index is integer) or a collection
        of distributions (index is a python sequence) ranked according to criterion.
        This last one should be 'BIC' or 'KS', default is 'BIC'.
        The default index is 0.

        Parameters
        ----------
        index : int or python sequence
        criterion : string
            Must be either 'BIC' or 'KS'.  Default is 'BIC'.

        Returns
        -------
        out : OpenTURNS Distribution or OpenTURNS DistributionCollection

        Example
        -------
        >>> import openturns as ot
        >>> from easyfitting import FitContinuousDistribution1D
        >>> x = ot.Normal().getSample(100)
        >>> fit = FitContinuousDistribution1D(x, 0.10)
        >>> # best bic distribution
        >>> bestDistribution = fit.getBestDistribution(0, 'BIC')
        >>> # Similar to
        >>> bestDistribution = fit.getBestDistribution()
        >>> # Get the best KS collection
        >>> bestDistribution = fit.getBestDistribution(0, 'KS')
        >>> # Get a collection of the two best distributions
        >>> bestDistribution = fit.getBestDistribution([0,1], 'BIC')

        '''
        assert (isinstance(index, int) or isinstance(index, tuple) or isinstance(index, list))
        uppercriterion = self.__checkCriterionArg(criterion)
        sorted_list = self._getSortedCatalog(uppercriterion)
        size = len(sorted_list)
        if isinstance(index, int):
            if index >= size:
                raise ValueError('Only ' + str(size) + ' distributions have been tested')
            distReturned = sorted_list[index]
            if distReturned.status == 0:
                ot.Log.Warn('Care! The distribution has been rejected by the KS test')
            return distReturned.distribution
        else:   # python sequence
            if max(index) >= size:
                raise ValueError('Only ' + str(size) + ' distributions have been tested')
            collection = ot.DistributionCollection()
            for point in index:
                distReturned = sorted_list[point]
                if distReturned.status == 0:
                    ot.Log.Warn('Care! The distribution has been rejected by the KS test')
                collection.add(distReturned.distribution)
            return collection

    def getTestedDistribution(self, criterion='BIC'):
        """
        Return the list of distributions that have been tested.

        The returned list (DistributionCollection) is ranked according to the
        BIC or Kolmogorov criterion.  The argument fixes the way to rank the
        collection.

        Parameters
        ----------
        criterion : string
            Must be either 'BIC' or 'KS'.  Default is 'BIC'.

        Returns
        -------
        A DistributionCollection instance.

        Example
        -------
        >>> import openturns as ot
        >>> from easyfitting import FitContinuousDistribution1D
        >>> sample = ot.Uniform().getSample(100)
        >>> fit = FitContinuousDistribution1D(sample)
        >>> # All tested distributions
        >>> testedDistribution = fit.getTestedDistribution('BIC')
        >>> # or
        >>> testedDistribution = fit.getTestedDistribution()
        >>> # KS p-values as criterion of ranking
        >>> testedDistribution = fit.getTestedDistribution('KS')

        """
        uppercriterion = self.__checkCriterionArg(criterion)
        index = len(self._catalog)
        return self.getBestDistribution(range(index), uppercriterion)

    def printAcceptedDistribution(self, criterion="BIC"):
        """
        Print the list of distributions that have been tested and accepted
        according to the Kolmogorov test.

        The print is done using the following scheme:
          Distribution name | Value1 | Value2
        where:
          Distribution name : Pretty print of the distribution name
          Value1            : pValue of the Kolmogorov test
          Value2            : BIC value

        Parameters
        ----------
        criterion : string
            Must be either 'BIC' or 'KS'.  Default is 'BIC'.

        Returns
        -------
        Nothing.

        Example
        -------
        >>> import openturns as ot
        >>> from easyfitting import FitContinuousDistribution1D
        >>> sample = ot.Uniform(0, 1).getSample(100)
        >>> fit = FitContinuousDistribution1D(sample)
        >>> fit.printAcceptedDistribution('BIC')
        >>> fit.printAcceptedDistribution()
        >>> fit.printAcceptedDistribution('KS')

        """
        uppercriterion = self.__checkCriterionArg(criterion)
        sorted_list = self._getSortedCatalog(uppercriterion)
        max_len = 0
        for key in sorted_list:
            if key.status == 1:
                max_len = max(max_len, len(str(key.distribution)))
        for key in sorted_list:
            if key.status == 1:
                print(str(key.distribution) + \
                    ' '*(max_len + 1 - len(str(key.distribution))) + \
                    '\t' + str(round(key.pvalue, 3)) + \
                    '\t' + str(round(key.bic, 3)))

    def printExceptedDistribution(self):
        """
        Print the distributions that have not been tested, with the reason
        explaining why.
        """
        print('---------------- ' \
            + 'NOT TESTED DISTRIBUTIONS' \
            + '  -------------------------------')
        for key in self._catalog:
            if key.status == 2:
                print(key.class_name+" - "+key.message)
        print('-' * 74)

    def printTestedDistribution(self, criterion="BIC"):
        """
        Print the list of distributions that have been tested.

        The print is done using the following scheme:
          Distribution name | Boolean | Value1 | Value2
        where :
          Distribution name : Pretty print of the distribution name
          Boolean           : Boolean value (Kolmogorov test Accepted / Rejected)
          Value1            : pValue of the Kolmogorov test
          Value2            : BIC value

        Parameters
        ----------
        criterion : string
            Must be either 'BIC' or 'KS'.  Default is 'BIC'.

        Returns
        -------
        Nothing

        Example
        -------
        >>> import openturns as ot
        >>> from easyfitting import FitContinuousDistribution1D
        >>> sample = ot.Uniform(0, 1).getSample(100)
        >>> fit = FitContinuousDistribution1D(sample)
        >>> fit.printTestedDistribution('BIC')
        >>> fit.printTestedDistribution()
        >>> fit.printTestedDistribution('KS')

        """
        uppercriterion = self.__checkCriterionArg(criterion)
        sorted_list = self._getSortedCatalog(uppercriterion)
        acceptedstr = ['Rejected', 'Accepted', 'Rejected']
        max_len = 0
        for key in sorted_list:
            if key.status != 2:
                max_len = max(max_len, len(str(key.distribution)))
        for key in sorted_list:
            if key.status != 2:
                print(str(key.distribution) + \
                    ' '*(max_len + 1 - len(str(key.distribution))) + \
                    '\t' + acceptedstr[key.status] + \
                    '\t' + str(round(key.pvalue, 3)) + \
                    '\t' + str(round(key.bic, 3)))
