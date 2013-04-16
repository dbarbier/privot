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
    Validation of get_points_on_surface_grid_ function for index in 0, 10
    Validation in 1D, 2D and 3D cases
     1) Validation of theoretical number of points
     2) Validation of unicity of points
    ----

    """
    # 1D distribution
    collection_list_1D = []
    collection1D = ot.DistributionCollection([ot.Uniform()])
    matrix1D = ot.Matrix([[1.0]])
    distribution1D = MV.PythonMultivariateRandomMixture(collection1D, matrix1D)
    print "1D case"
    for i in xrange(1, index):
        list_points1D = distribution1D.get_points_on_surface_grid_(i)
        collection_list_1D.append(list_points1D)
        print "index=%d #list=%d #expected=%d" %(i, len(list_points1D), 2)
        unique = 0
        for j, pt in enumerate(list_points1D):
            unique = max(unique, list_points1D.count(pt))
        print "Point #%d, count in list =#%d" %(j + 1, unique)

    # 2D distribution
    collection_list_2D = []
    collection2D = ot.DistributionCollection([ot.Normal(2.0, 3.0), ot.Normal(1.0, 4.0)])
    matrix2D = ot.Matrix([[4, 2], [1, 1.4]])
    distribution2D = MV.PythonMultivariateRandomMixture(collection2D, matrix2D)
    print
    print "2D case"
    for i in xrange(1, index):
        list_points2D = distribution2D.get_points_on_surface_grid_(i)
        collection_list_2D.append(list_points2D)
        print "index=%d #list=%d #expected=%d" %(i, len(list_points2D), 8*i)
        unique = 0
        for j, pt in enumerate(list_points2D):
            unique = max(unique, list_points2D.count(pt))
        print "Point #%d, count in list =#%d" %(j + 1, unique)

    # Graphical validation using matplotlib
    try :
        import matplotlib.pylab as plt
        from matplotlib.backends.backend_pdf import PdfPages
        pdf_file = 'validation_2D.pdf'
        pdf_plot = PdfPages(pdf_file)
        for list_points2D in collection_list_2D:
            x = [list(point)[0] for point in list_points2D]
            y = [list(point)[1] for point in list_points2D]
            fig = plt.figure()
            plt.xlim(-index, index)
            plt.ylim(-index, index)
            plt.plot(x, y, '.')
            plt.xlabel('x')
            plt.ylabel('y')
            plt.title('index = %d' %max(x))
            pdf_plot.savefig(fig)
            plt.close()
        pdf_plot.close()
    except ImportError:
        import warnings
        warnings.warn('No graphical validation. Install matplotlib for that purpose')

    # 3D distribution
    collection_list_3D = []
    collection3D = ot.DistributionCollection([ot.Normal(2.0, 3.0), ot.Normal(1.0, 4.0)])
    matrix3D = ot.Matrix([[4, 2], [1, 1.4], [-4.5, 6.0]])
    distribution3D = MV.PythonMultivariateRandomMixture(collection3D, matrix3D)
    print
    print "3D case"
    for i in xrange(1, index):
        list_points3D = distribution3D.get_points_on_surface_grid_(i)
        collection_list_3D.append(list_points3D)
        print "index=%d #list=%d #expected=%d" %(i, len(list_points3D), 24 *i *i + 2)
        unique = 0
        for j, pt in enumerate(list_points3D):
            unique = max(unique, list_points3D.count(pt))
        print "Point #%d, count in list =#%d" %(j + 1, unique)
    # Graphical validation using matplotlib
    try :
        import matplotlib.pylab as plt
        from mpl_toolkits.mplot3d import Axes3D
        from matplotlib.backends.backend_pdf import PdfPages
        pdf_file = 'validation_3D.pdf'
        pdf_plot = PdfPages(pdf_file)
        for list_points3D in collection_list_3D:
            x = [list(point)[0] for point in list_points3D]
            y = [list(point)[1] for point in list_points3D]
            z = [list(point)[2] for point in list_points3D]
            fig = plt.figure()
            ax = fig.add_subplot(111, projection='3d')
            ax.scatter(x, y, z, c='b', marker='s')
            ax.set_xlabel('X Label')
            ax.set_ylabel('Y Label')
            ax.set_zlabel('Z Label')
            ax.set_xlim(-index, index)
            ax.set_ylim(-index, index)
            ax.set_zlim(-index, index)
            ax.set_title('index = %d' %max(x))
            pdf_plot.savefig(fig)
            plt.close()
        pdf_plot.close()
    except ImportError:
        import warnings
        warnings.warn('No graphical validation. Install matplotlib for that purpose')
