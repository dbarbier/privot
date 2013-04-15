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
    class.

"""

if __name__ == "__main__":
    import openturns as ot
    import MultivariateRandomMixture as MV
    index = 10

    """
    Validation of get_points_on_grid_pdf_ function for index in 0, 10
    Validation in 1D, 2D and 3D cases
     1) Validation of theoretical number of points
     2) Validation of unicity of points
    ----

    """
    # 1D distribution
    collection1D = ot.DistributionCollection([ot.Uniform()])
    matrix1D = ot.Matrix([[1.0]])
    distribution1D = MV.PythonMultivariateRandomMixture(collection1D, matrix1D)
    print "1D case"
    for i in xrange(1, index):
        list_points = distribution1D.get_points_on_grid_pdf_(i)
        print "index=%d #list=%d #expected=%d" %(i, len(list_points), 2)
        unique = 0
        for j, pt in enumerate(list_points):
            unique = max(unique, list_points.count(pt))
        print "Point #%d, count in list =#%d" %(j + 1, unique)

    # 2D distribution
    collection2D = ot.DistributionCollection([ot.Normal(2.0, 3.0), ot.Normal(1.0, 4.0)])
    matrix2D = ot.Matrix([[4, 2], [1, 1.4]])
    distribution2D = MV.PythonMultivariateRandomMixture(collection2D, matrix2D)
    print "2D case"
    for i in xrange(1, index):
        list_points = distribution2D.get_points_on_grid_pdf_(i)
        print "index=%d #list=%d #expected=%d" %(i, len(list_points), 8*i)
        unique = 0
        for j, pt in enumerate(list_points):
            unique = max(unique, list_points.count(pt))
        print "Point #%d, count in list =#%d" %(j + 1, unique)

    # 3D distribution
    collection3D = ot.DistributionCollection([ot.Normal(2.0, 3.0), ot.Normal(1.0, 4.0)])
    matrix3D = ot.Matrix([[4, 2], [1, 1.4], [-4.5, 6.0]])
    distribution3D = MV.PythonMultivariateRandomMixture(collection3D, matrix3D)
    print "3D case"
    for i in xrange(1, index):
        list_points = distribution3D.get_points_on_grid_pdf_(i)
        print "index=%d #list=%d #expected=%d" %(i, len(list_points), 24 *i *i + 2)
        unique = 0
        for j, pt in enumerate(list_points):
            unique = max(unique, list_points.count(pt))
        print "Point #%d, count in list =#%d" %(j + 1, unique)
