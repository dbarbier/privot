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
#    get_skin_iterator(self, index)
#    get_size_of_level(self, index)
#    get_size_upto_level(self, index)


class Cube1D:
    """
    Trivial implementation of a 1D grid iterator.

    Example
    -------
    >>> import MaxNormMeshGrid
    >>> meshgrid = MaxNormMeshGrid.Cube1D([0.1], True)
    >>> for i in xrange(1, 10):
    >>>     iterator = meshgrid.get_skin_iterator(i)
    >>>     print("Iteration "+str(i))
    >>>     try:
    >>>         while True:
    >>>             y = iterator.next()
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
        if len(steps) != 1:
            raise ValueError("Argument must be a vector of dimension 1")
        self.step_ = steps[0]
        self.symmetric_ = symmetric

    def clone(self, steps):
        return Cube1D(steps, self.symmetric_)

    def isSymmetric(self):
        return self.symmetric_

    @staticmethod
    def dot(x, y):
        return x[0] * y[0]

    def get_skin_iterator(self, index):
        if index == 0:
            yield (0.,)
        else:
            if self.symmetric_:
                yield (index * self.step_,)
            else:
                yield (-index * self.step_,)
                yield (index * self.step_,)

    def get_skin_iterator_evaluate(self, index, function):
        iterator = self.get_skin_iterator(index)
        try:
            while True:
                point = iterator.next()
                yield point, function(point)
        except StopIteration:
            raise StopIteration()

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
    Trivial implementation of a 2D grid iterator.
    Points at distance N are found by considering all points in the cube (N,N) which does not
    belong to the cube (N-1,N-1).  Complexity of get_skin_iterator is O(N^2).

    """

    def __init__(self, steps, symmetric = True):
        if len(steps) != 2:
            raise ValueError("Argument must be a vector of dimension 2")
        self.stepX_ = steps[0]
        self.stepY_ = steps[1]
        self.symmetric_ = symmetric

    def clone(self, steps):
        return Cube2D(steps, self.symmetric_)

    def isSymmetric(self):
        return self.symmetric_

    @staticmethod
    def dot(x, y):
        return x[0] * y[0] + x[1] * y[1]

    def get_skin_iterator(self, index):
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

    def get_skin_iterator_evaluate(self, index, function):
        iterator = self.get_skin_iterator(index)
        try:
            while True:
                point = iterator.next()
                yield point, function(point)
        except StopIteration:
            raise StopIteration()

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
    Optimized implementation of a 2D grid iterator.
    Points at distance N are found by considering the 4 lines |x|=N and |y|=N.
    Complexity of get_skin_iterator is O(N).

    """

    def __init__(self, steps, symmetric = True):
        if len(steps) != 2:
            raise ValueError("Argument must be a vector of dimension 2")
        self.stepX_ = steps[0]
        self.stepY_ = steps[1]
        self.symmetric_ = symmetric

    def clone(self, steps):
        return SkinCube2D(steps, self.symmetric_)

    def isSymmetric(self):
        return self.symmetric_

    @staticmethod
    def dot(x, y):
        return x[0] * y[0] + x[1] * y[1]

    def get_skin_iterator(self, index):
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

    def get_skin_iterator_evaluate(self, index, function):
        iterator = self.get_skin_iterator(index)
        try:
            while True:
                point = iterator.next()
                yield point, function(point)
        except StopIteration:
            raise StopIteration()

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
    Trivial implementation of a 3D grid iterator.
    Points at distance N are found by considering all points in the cube (N,N,N) which does not
    belong to the cube (N-1,N-1,N-1).  Complexity of get_skin_iterator is O(N^3).

    """

    def __init__(self, steps, symmetric = True):
        if len(steps) != 3:
            raise ValueError("Argument must be a vector of dimension 3")
        self.stepX_ = steps[0]
        self.stepY_ = steps[1]
        self.stepZ_ = steps[2]
        self.symmetric_ = symmetric

    def clone(self, steps):
        return Cube3D(steps, self.symmetric_)

    def isSymmetric(self):
        return self.symmetric_

    @staticmethod
    def dot(x, y):
        return x[0] * y[0] + x[1] * y[1] + x[2] * y[2]

    def get_skin_iterator(self, index):
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

    def get_skin_iterator_evaluate(self, index, function):
        iterator = self.get_skin_iterator(index)
        try:
            while True:
                point = iterator.next()
                yield point, function(point)
        except StopIteration:
            raise StopIteration()

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
    Optimized implementation of a 3D grid iterator.
    Points at distance N are found by considering the 6 planes |x|=N, |y|=N and |z|=N.
    Complexity of get_skin_iterator is O(N^2).

    """

    def __init__(self, steps, symmetric = True):
        if len(steps) != 3:
            raise ValueError("Argument must be a vector of dimension 3")
        self.stepX_ = steps[0]
        self.stepY_ = steps[1]
        self.stepZ_ = steps[2]
        self.symmetric_ = symmetric

    def clone(self, steps):
        return SkinCube3D(steps, self.symmetric_)

    def isSymmetric(self):
        return self.symmetric_

    @staticmethod
    def dot(x, y):
        return x[0] * y[0] + x[1] * y[1] + x[2] * y[2]

    def get_skin_iterator(self, index):
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

    def get_skin_iterator_evaluate(self, index, function):
        iterator = self.get_skin_iterator(index)
        try:
            while True:
                point = iterator.next()
                yield point, function(point)
        except StopIteration:
            raise StopIteration()

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

class CachedMeshGrid:
    """
    Implements a cache.

    """

    def __init__(self, meshGrid, size=100000):
        self.meshGrid_ = meshGrid
        self.maxSize_ = size
        self.currentSize_ = 0
        self.cachedPosition_ = []
        self.cachedValue_ = []

    def clone(self, steps):
        return CachedMeshGrid(self.meshGrid_.clone(steps), self.currentSize_)

    def setCacheSize(self, size):
        if size < self.currentSize_:
            del self.cachedPosition_[size:]
            del self.cachedValue_[size:]
            self.currentSize_ = size
        self.maxSize_ = size

    def isSymmetric(self):
        return self.meshGrid_.isSymmetric()

    def dot(self, x, y):
        return self.meshGrid_.dot(x, y)

    def get_skin_iterator(self, index):
        if self.meshGrid_.get_size_upto_level(index+1) > self.maxSize_:
            #  For simplicity reason, if this level does not fully fit
            #  into the cache, it is not stored
            iterator = self.meshGrid_.get_skin_iterator(index)
            try:
                while True:
                    yield iterator.next()
            except StopIteration:
                raise StopIteration()
        else:
            #  Find the first level not being cached
            start = index + 1
            while start > 0 and self.meshGrid_.get_size_upto_level(start) > self.currentSize_:
                start -= 1

            #  Fill up the cache
            for  i in xrange(start, index + 1):
                iterator = self.meshGrid_.get_skin_iterator(i)
                try:
                    while True:
                        self.cachedPosition_.append(iterator.next())
                except StopIteration:
                    pass
                self.currentSize_ += self.meshGrid_.get_size_of_level(i)

            for x in xrange(self.meshGrid_.get_size_upto_level(index), self.meshGrid_.get_size_upto_level(index)+self.meshGrid_.get_size_of_level(index)):
                yield self.cachedPosition_[x]

    def get_skin_iterator_evaluate(self, index, function):
        if self.meshGrid_.get_size_upto_level(index+1) > self.maxSize_:
            #  For simplicity reason, if this level does not fully fit
            #  into the cache, it is not stored
            iterator = self.meshGrid_.get_skin_iterator(index)
            try:
                while True:
                    point = iterator.next()
                    yield (point, function(point))
            except StopIteration:
                raise StopIteration()
        else:
            #  Find the first level not being cached
            start = index + 1
            while start > 0 and self.meshGrid_.get_size_upto_level(start) > self.currentSize_:
                start -= 1

            #  Fill up the cache
            for  i in xrange(start, index + 1):
                iterator = self.meshGrid_.get_skin_iterator(i)
                try:
                    while True:
                        point = iterator.next()
                        self.cachedPosition_.append(point)
                        self.cachedValue_.append(function(point))
                except StopIteration:
                    pass
                self.currentSize_ += self.meshGrid_.get_size_of_level(i)

            for x in xrange(self.meshGrid_.get_size_upto_level(index), self.meshGrid_.get_size_upto_level(index)+self.meshGrid_.get_size_of_level(index)):
                yield (self.cachedPosition_[x], self.cachedValue_[x])

    def get_size_of_level(self, index):
        return self.meshGrid_.get_size_of_level(index)

    def get_size_upto_level(self, index):
        return self.meshGrid_.get_size_upto_level(index)

    def __str__(self):
        return "CachedMeshGrid: maxItems="+str(self.maxSize_)+" usedItems="+str(self.currentSize_)+" mesherGrid="+str(self.meshGrid_)

