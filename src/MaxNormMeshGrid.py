#                                               -*- Python -*-
#
#  @file  MaxNormMeshGrid.py
#  @brief Helper classes to walk on a regular grid
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

#  This file defines classes to walk on a nD mesh grid (for n=1, 2 or 3)
#  by considering vertices which are at a given distance of origin using
#  the maximum norm.
#
#  All classes define the following methods:
#    __init__(self, steps, symmetric)
#    isSymmetric(self)
#    get_skin_walker(self, index)
#    get_size_of_level(self, index)
#    get_size_upto_level(self, index)


class Cube1D:
    """
    Trivial implementation of a 1D grid walker.

    Example
    -------
    >>> import MaxNormMeshGrid
    >>> meshgrid = MaxNormMeshGrid.Cube1D([0.1], True)
    >>> for i in xrange(1, 10):
    >>>     walker = meshgrid.get_skin_walker(i)
    >>>     print("Iteration "+str(i))
    >>>     try:
    >>>         while True:
    >>>             y = walker.next()
    >>>             print(" -> "+y.__str__())
    >>>     except StopIteration:
    >>>         pass

    """

    def __init__(self, steps, symmetric = True):
        """
        Parameters
        ----------
        steps: array containing one float
             The grid step

        symmetric: boolean
             If True (by default), only positive values are returned

        """
        if (len(steps) != 1):
            raise ValueError("Argument must be a vector of dimension 1")
        self.step_ = steps[0]
        self.symmetric_ = symmetric

    def isSymmetric(self):
        return self.symmetric_

    def get_skin_walker(self, index):
        if index == 0:
            yield (0.,)
        else:
            if self.symmetric_:
                yield (index * self.step_,)
            else:
                yield (-index * self.step_,)
                yield (index * self.step_,)

    def get_size_of_level(self, index):
        if index == 0:
            return 1
        elif self.symmetric_:
            return 1
        else:
            return 2

    def get_size_upto_level(self, index):
        if index <= 0:
            return 0
        elif self.symmetric_:
            return index
        else:
            return 2 * index - 1

    def __str__(self):
        return "Cube1D symmetric="+str(self.symmetric_)+" step="+str(self.step_)

class Cube2D:
    """
    Trivial implementation of a 2D grid walker.
    Points at distance N are found by considering all points in the cube (N,N) which does not
    belong to the cube (N-1,N-1).  Complexity of get_skin_walker is O(N^2).

    """

    def __init__(self, steps, symmetric = True):
        if (len(steps) != 2):
            raise ValueError("Argument must be a vector of dimension 2")
        self.stepX_ = steps[0]
        self.stepY_ = steps[1]
        self.symmetric_ = symmetric

    def isSymmetric(self):
        return self.symmetric_

    def get_skin_walker(self, index):
        if index == 0:
            yield (0., 0.)
        else:
            if self.symmetric_:
                yield (0., float(index) * self.stepY_)
                for ix in xrange(1, index + 1):
                    inner_x = (abs(ix) < index)
                    cx = float(ix) * self.stepX_
                    for iy in xrange(-index, index+1):
                        inner_y = (abs(iy) < index)
                        if not(inner_x and inner_y):
                            yield (cx, float(iy) * self.stepY_)
            else:
                for ix in xrange(-index, index + 1):
                    inner_x = (abs(ix) < index)
                    cx = float(ix) * self.stepX_
                    for iy in xrange(-index, index+1):
                        inner_y = (abs(iy) < index)
                        if not(inner_x and inner_y):
                            yield (cx, float(iy) * self.stepY_)

    def get_size_of_level(self, index):
        if index == 0:
            return 1
        elif self.symmetric_:
            return 4 * index
        else:
            return 8 * index

    def get_size_upto_level(self, index):
        if index <= 0:
            return 0
        elif self.symmetric_:
            return 2 * index * (index - 1) + 1
        else:
            return (2 * index - 1) * (2 * index - 1)

    def __str__(self):
        return "Cube2D symmetric="+str(self.symmetric_)+" step=["+str(self.stepX_)+", "+str(self.stepY_)+"]"

class SkinCube2D:
    """
    Optimized implementation of a 2D grid walker.
    Points at distance N are found by considering the 4 lines |x|=N and |y|=N.
    Complexity of get_skin_walker is O(N).

    """

    def __init__(self, steps, symmetric = True):
        if (len(steps) != 2):
            raise ValueError("Argument must be a vector of dimension 2")
        self.stepX_ = steps[0]
        self.stepY_ = steps[1]
        self.symmetric_ = symmetric

    def isSymmetric(self):
        return self.symmetric_

    def get_skin_walker(self, index):
        if index == 0:
            yield (0., 0.)
        else:
            if self.symmetric_:
                # 1) right
                cy = float(index) * self.stepY_
                for ix in range(-index, index):
                    yield (float(ix) * self.stepX_, cy)
                # 2) top
                cx = float(index) * self.stepX_
                for iy in range(index, -index, -1):
                    yield (cx, float(iy) * self.stepY_)
            else:
                # 1) bottom
                cx = float(-index) * self.stepX_
                for iy in range(-index, index):
                    yield (cx, float(iy) * self.stepY_)
                # 2) right
                cy = float(index) * self.stepY_
                for ix in range(-index, index):
                    yield (float(ix) * self.stepX_, cy)
                # 3) top
                cx = float(index) * self.stepX_
                for iy in range(index, -index, -1):
                    yield (cx, float(iy) * self.stepY_)
                # 4) left hand
                cy = float(-index) * self.stepY_
                for ix in range(index, -index, -1):
                    yield (float(ix) * self.stepX_, cy)

    def get_size_of_level(self, index):
        if index == 0:
            return 1
        elif self.symmetric_:
            return 4 * index
        else:
            return 8 * index

    def get_size_upto_level(self, index):
        if index <= 0:
            return 0
        elif self.symmetric_:
            return 2 * index * (index - 1) + 1
        else:
            return (2 * index - 1) * (2 * index - 1)

    def __str__(self):
        return "SkinCube2D symmetric="+str(self.symmetric_)+" step=["+str(self.stepX_)+", "+str(self.stepY_)+"]"

class Cube3D:
    """
    Trivial implementation of a 3D grid walker.
    Points at distance N are found by considering all points in the cube (N,N,N) which does not
    belong to the cube (N-1,N-1,N-1).  Complexity of get_skin_walker is O(N^3).

    """

    def __init__(self, steps, symmetric = True):
        if (len(steps) != 3):
            raise ValueError("Argument must be a vector of dimension 3")
        self.stepX_ = steps[0]
        self.stepY_ = steps[1]
        self.stepZ_ = steps[2]
        self.symmetric_ = symmetric

    def isSymmetric(self):
        return self.symmetric_

    def get_skin_walker(self, index):
        if index == 0:
            yield (0., 0., 0.)
        else:
            if self.symmetric_:
                yield (0., 0., float(index) * self.stepZ_)
                for iy in xrange(1, index+1):
                    inner_y = (abs(iy) < index)
                    cy = float(iy) * self.stepY_
                    for iz in xrange(-index, index+1):
                        inner_z = (abs(iz) < index)
                        if not(inner_y and inner_z):
                            yield (0., cy, float(iz) * self.stepZ_)
                for ix in xrange(1, index+1):
                    inner_x = (abs(ix) < index)
                    cx = float(ix) * self.stepX_
                    for iy in xrange(-index, index+1):
                        inner_y = (abs(iy) < index)
                        cy = float(iy) * self.stepY_
                        for iz in xrange(-index, index+1):
                            inner_z = (abs(iz) < index)
                            if not(inner_x and inner_y and inner_z):
                                yield (cx, cy, float(iz) * self.stepZ_)
            else:
                for ix in xrange(-index, index+1):
                    inner_x = (abs(ix) < index)
                    cx = float(ix) * self.stepX_
                    for iy in xrange(-index, index+1):
                        inner_y = (abs(iy) < index)
                        cy = float(iy) * self.stepY_
                        for iz in xrange(-index, index+1):
                            inner_z = (abs(iz) < index)
                            if not(inner_x and inner_y and inner_z):
                                yield (cx, cy, float(iz) * self.stepZ_)

    def get_size_of_level(self, index):
        if index == 0:
            return 1
        elif self.symmetric_:
            return 12 * index * index + 1
        else:
            return 24 * index * index + 2

    def get_size_upto_level(self, index):
        if index <= 0:
            return 0
        elif self.symmetric_:
            return index * (1 + (2 * index - 1) * (2 * index - 2))
        else:
            return (2 * index - 1) * (2 * index - 1) * (2 * index - 1)

    def __str__(self):
        return "Cube3D symmetric="+str(self.symmetric_)+" step=["+str(self.stepX_)+", "+str(self.stepY_)+", "+str(self.stepZ_)+"]"

class SkinCube3D:
    """
    Optimized implementation of a 3D grid walker.
    Points at distance N are found by considering the 6 planes |x|=N, |y|=N and |z|=N.
    Complexity of get_skin_walker is O(N^2).

    """

    def __init__(self, steps, symmetric = True):
        if (len(steps) != 3):
            raise ValueError("Argument must be a vector of dimension 3")
        self.stepX_ = steps[0]
        self.stepY_ = steps[1]
        self.stepZ_ = steps[2]
        self.symmetric_ = symmetric

    def isSymmetric(self):
        return self.symmetric_

    def get_skin_walker(self, index):
        if index == 0:
            yield (0., 0., 0.)
        else:
            if self.symmetric_:
                # 1) contour (like 2D case) for each z
                cx = float(index) * self.stepX_
                cy = float(index) * self.stepY_
                for iz in xrange(-index, index + 1):
                    cz = float(iz) * self.stepZ_
                    # b) right
                    for iy in range(-index, index):
                        yield (  cx, float(iy) * self.stepY_, cz)
                    # c) top
                    for ix in range(index, -index, -1):
                        yield (float(ix) * self.stepX_,   cy, cz)
                # 2) quasi-plain square for |z| = index
                # quasi because we remove corners and contours already in the list
                cz = float(index) * self.stepZ_
                for ix in xrange(-index + 1 , index):
                    cx = float(ix) * self.stepX_
                    for iy in xrange(-index + 1, index):
                      yield (cx, float(iy) * self.stepY_, cz)
            else:
                # 1) contour (like 2D case) for each z
                cx = float(index) * self.stepX_
                cy = float(index) * self.stepY_
                for iz in xrange(-index, index + 1):
                    cz = float(iz) * self.stepZ_
                    # a) bottom
                    for ix in range(-index, index):
                        yield (float(ix) * self.stepX_, - cy, cz)
                    # b) right
                    for iy in range(-index, index):
                        yield (  cx, float(iy) * self.stepY_, cz)
                    # c) top
                    for ix in range(index, -index, -1):
                        yield (float(ix) * self.stepX_,   cy, cz)
                    # d) left hand
                    for iy in range(index, -index, -1):
                        yield (- cx, float(iy) * self.stepY_, cz)
                # 2) quasi-plain square for |z| = index
                # quasi because we remove corners and contours already in the list
                for iz in [-index, index]:
                    cz = float(iz) * self.stepZ_
                    for ix in xrange(-index + 1 , index):
                        cx = float(ix) * self.stepX_
                        for iy in xrange(-index + 1, index):
                          yield (cx, float(iy) * self.stepY_, cz)

    def get_size_of_level(self, index):
        if index == 0:
            return 1
        elif self.symmetric_:
            return 12 * index * index + 1
        else:
            return 24 * index * index + 2

    def get_size_upto_level(self, index):
        if index <= 0:
            return 0
        elif self.symmetric_:
            return index * (1 + (2 * index - 1) * (2 * index - 2))
        else:
            return (2 * index - 1) * (2 * index - 1) * (2 * index - 1)

    def __str__(self):
        return "SkinCube3D symmetric="+str(self.symmetric_)+" step=["+str(self.stepX_)+", "+str(self.stepY_)+", "+str(self.stepZ_)+"]"

