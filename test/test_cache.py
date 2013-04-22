#                                               -*- Python -*-
#
#  @file  test_2D_elementary_functions.py
#  @brief MultivariateRandomMixture validation tests
#
#  Copyright (C) 2013 EADS IW France
#
#  Author(s) :  Denis Barbier, IMACS
#               Sofiane Haddad, IMACS
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
    Tests of MultivariateRandomMixture elementary functions
    =======================================================
    Validation of the some methods of the PythonMultivariateRandomMixture
    clas ==> cache mechanism used for the computation of 
    computeDeltaCharacteristicFunction

"""

if __name__ == "__main__":
    import openturns as ot
    import MultivariateRandomMixture as MV
    import numpy as np
    import time


    """
    Validation of computeDeltaCharacteristicFunction

    """
    # distribution
    collection = ot.DistributionCollection([ot.Uniform(-50, -35), ot.Uniform(35, 50)])
    matrix = ot.Matrix([2 * [1.0]])
    distribution = MV.PythonMultivariateRandomMixture(collection, matrix)
    xmin, xmax, dx = 0, 100, distribution.getReferenceBandwidth()[0]
    x = np.arange(xmin, xmax + dx, dx)
    t1 = []
    t2 = []
    err = 0.0
    for i, value in enumerate(x):
        tic = time.time()
        c1 = distribution.computeDeltaCharacteristicFunction([value])
        toc = time.time()
        dt1 = toc - tic
        t1.append(dt1)
        tic = time.time()
        c2 = distribution.computeDeltaCharacteristicFunctionCache(i)
        toc = time.time()
        try:
            c2 = c2[0]
        except TypeError:
            pass
        dt2 = toc - tic
        t2.append(dt2)
        print "time comparison: without cache=%s, with cache=%s, ratio=%s" %(dt1, dt2, dt1/dt2)
        try :
            r = abs((c1 - c2)/(c2))
        except ZeroDivisionError:
            r = 0.0
        print "delta values comparison : without cache=%s, with cache=%s, error=%s" %(c1, c2, r)
        err += r*r
    print "total error = %s" %err
