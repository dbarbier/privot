#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try :
    mesh1D =  Mesh()
    print "Default 1D mesh=", mesh1D
    vertices = NumericalSample(0, 1)
    vertices.add(NumericalPoint(1, 0.5))
    vertices.add(NumericalPoint(1, 1.5))
    vertices.add(NumericalPoint(1, 2.1))
    vertices.add(NumericalPoint(1, 2.7))
    simplicies = IndicesCollection(3, Indices(2))
    simplicies[0][0] = 0
    simplicies[0][1] = 1
    simplicies[1][0] = 1
    simplicies[1][1] = 2
    simplicies[2][0] = 2
    simplicies[2][1] = 3
    mesh1D = Mesh(vertices, simplicies)
    print "1D mesh=", mesh1D
    print "Is empty? ", mesh1D.isEmpty()
    print "vertices=", mesh1D.getVertices()
    print "simplices=", mesh1D.getSimplices()
    print "volume=", mesh1D.getVolume()
    print "First simplex volume=", mesh1D.computeSimplexVolume(0)
    p = NumericalPoint(1)
    p[0] = 1.3
    print "is p=", p, " in mesh? ", mesh1D.contains(p)
    mesh1D.draw().draw("mesh1D")
    vertices = NumericalSample(0, 2)
    p = NumericalPoint(2)

    p[0] = 0.0
    p[1] = 0.0
    vertices.add(p)

    p[0] = 1.0
    p[1] = 0.0
    vertices.add(p)

    p[0] = 1.0
    p[1] = 1.0
    vertices.add(p)

    p[0] = 1.5
    p[1] = 1.0
    vertices.add(p)

    p[0] = 2.0
    p[1] = 1.5
    vertices.add(p)

    p[0] = 0.5
    p[1] = 1.5
    vertices.add(p)
    simplicies = IndicesCollection(5, Indices(3))
    simplicies[0][0] = 0
    simplicies[0][1] = 1
    simplicies[0][2] = 2

    simplicies[1][0] = 1
    simplicies[1][1] = 2
    simplicies[1][2] = 3

    simplicies[2][0] = 2
    simplicies[2][1] = 3
    simplicies[2][2] = 4

    simplicies[3][0] = 2
    simplicies[3][1] = 4
    simplicies[3][2] = 5

    simplicies[4][0] = 0
    simplicies[4][1] = 2
    simplicies[4][2] = 5
    mesh2D = Mesh(vertices, simplicies)
    print "2D mesh=", mesh2D
    mesh2D.draw().draw("mesh2D")
    p[0] = 1.3
    p[1] = 0.8
except :
    import sys
    print "t_Mesh_std.py", sys.exc_type, sys.exc_value
