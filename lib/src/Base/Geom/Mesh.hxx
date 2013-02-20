//                                               -*- C++ -*-
/**
 *  @file  Mesh.hxx
 *  @brief Mesh is defined as the cartesian product of n 1D meshls ]low_1, up_1]x...x]low_n,up_n]
 *
 *  Copyright (C) 2005-2013 EDF-EADS-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @author schueller
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 *  Id      Mesh.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_MESH_HXX
#define OPENTURNS_MESH_HXX

#include "PersistentObject.hxx"
#include "Collection.hxx"
#include "PersistentCollection.hxx"
#include "NumericalPoint.hxx"
#include "Indices.hxx"
#include "SquareMatrix.hxx"
#include "DomainImplementation.hxx"
#include "Graph.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class Mesh
 *
 * A class that holds a mesh
 */
class Mesh
  : public DomainImplementation
{
  CLASSNAME;

public:
  typedef Collection< Indices >           IndicesCollection;
  typedef PersistentCollection< Indices > IndicesPersistentCollection;

  /** Default constructor */
  explicit Mesh(const UnsignedLong dimension = 1);

  /** Parameters constructor */
  Mesh(const NumericalSample & vertices,
       const IndicesCollection & simplices);

  /** Virtual constructor method */
  Mesh * clone() const;

  /** Check if the mesh is empty, i.e. if we have lowerBound >= upperBound for at least one component */
  Bool isEmpty() const;

  /** Check if the mesh is numerically empty, i.e. if we have lowerBound >= upperBound for at least one component */
  Bool isNumericallyEmpty() const;

  /** Check if the given point is inside of the closed mesh */
  Bool contains(const NumericalPoint & point) const;

  /** Check if the given point is numerically inside of the closed mesh, i.e. using only the bounds part of the mesh */
  Bool numericallyContains(const NumericalPoint & point) const;

  /** Get the numerical volume of the mesh */
  NumericalScalar getNumericalVolume() const;

  /** Comparison operator */
  Bool operator == (const Mesh & rhs) const ;

  /** Check mesh validity, i.e:
      non-overlaping simplices,
      no unused vertex,
      no simplices with duplicate vertices,
      no coincident vertices */
  Bool isValid() const;

  /** Build the affine matrix associated with a given simplex */
  SquareMatrix buildSimplexMatrix(const UnsignedLong index) const;

  /** Check if the given point is in the given simplex */
  Bool checkPointInSimplex(const NumericalPoint & point,
                           const UnsignedLong index) const;

  /** Vertices accessor */
  NumericalSample getVertices() const;
  void setVertices(const NumericalSample & vertices);

  /** Vertex accessor */
  NumericalPoint getVertex(const UnsignedLong index) const;
  void setVertex(const UnsignedLong index,
                 const NumericalPoint & vertex);

  /** Simplices accessor */
  IndicesCollection getSimplices() const;
  void setSimplices(const IndicesCollection & simplices);

  /** Simplex accessor */
  Indices getSimplex(const UnsignedLong index) const;

  /** Volume accessor */
  NumericalScalar getVolume() const;

  // Compute the volume of a given simplex
  NumericalScalar computeSimplexVolume(const UnsignedLong index) const;

  /** Drawing method */
  Graph draw() const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

  /** FreeFem mesh import */
  static Mesh ImportFromMSHFile(const String & fileName);

private:

  // Compute the total volume of the mesh
  void computeVolume() const;

  // An n-D mesh is a set of vertices with a topology described by a set of simplices
  // The vertices
  NumericalSample vertices_;

  // The simplices
  IndicesPersistentCollection simplices_;

  // The global volume
  mutable NumericalScalar volume_;

  // Flag to tell if the global volume has already been computed
  mutable Bool isAlreadyComputedVolume_;
}; /* class Mesh */

} /* Namespace OpenTURNS */

#endif /* OPENTURNS_MESH_HXX */
