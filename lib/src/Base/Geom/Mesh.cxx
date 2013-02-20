//                                               -*- C++ -*-
/**
 *  @file  Mesh.cxx
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 */
#include <fstream>

#include "Mesh.hxx"
#include "PersistentObjectFactory.hxx"
#include "Log.hxx"
#include "Os.hxx"
#include "Exception.hxx"
#include "Graph.hxx"
#include "Cloud.hxx"
#include "Curve.hxx"
#include "SpecFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Mesh);

static Factory<Mesh> RegisteredFactory("Mesh");

/* Default constructor */
Mesh::Mesh(const UnsignedLong dimension)
  : DomainImplementation(dimension)
  , vertices_()
  , simplices_()
  , volume_(0.0)
  , isAlreadyComputedVolume_(false)
{
  // Build a unique standard simplex
  // First, the vertices
  vertices_ = NumericalSample(dimension + 1, dimension);
  for (UnsignedLong i = 0; i < dimension; ++i) vertices_[i + 1][i] = 1.0;
  // Second, the connections
  simplices_ = IndicesCollection(1, Indices(dimension + 1));
  simplices_[0].fill();
}

/* Parameters constructor, simplified interface for 1D case */
Mesh::Mesh(const NumericalSample & vertices,
           const IndicesCollection & simplices)
  : DomainImplementation(vertices.getDimension())
  , vertices_(vertices)
  , simplices_(simplices)
  , volume_(0.0)
  , isAlreadyComputedVolume_(false)
{
  // Nothing to do
}

/* Clone method */
Mesh * Mesh::clone() const
{
  return new Mesh(*this);
}

/** Vertices accessor */
NumericalSample Mesh::getVertices() const
{
  return vertices_;
}

void Mesh::setVertices(const NumericalSample & vertices)
{
  vertices_ = vertices;
}

/* Vertex accessor */
NumericalPoint Mesh::getVertex(const UnsignedLong index) const
{
  if (index >= vertices_.getSize()) throw InvalidArgumentException(HERE) << "Error: the vertex index=" << index << " must be less than the number of vertices=" << vertices_.getSize();
  return vertices_[index];
}

void Mesh::setVertex(const UnsignedLong index,
                     const NumericalPoint & vertex)
{
  vertices_[index] = vertex;
}

/* Simplices accessor */
Mesh::IndicesCollection Mesh::getSimplices() const
{
  return simplices_;
}

void Mesh::setSimplices(const IndicesCollection & simplices)
{
  simplices_ = simplices;
}

/* Simplex accessor */
Indices Mesh::getSimplex(const UnsignedLong index) const
{
  if (index >= simplices_.getSize()) throw InvalidArgumentException(HERE) << "Error: the simplex index=" << index << " must be less than the number of simplices=" << simplices_.getSize();
  return simplices_[index];
}

/* Check if the mesh is empty, i.e if its numerical volume is zero */
Bool Mesh::isEmpty() const
{
  return isNumericallyEmpty();
}

/* Check if the mesh is empty, i.e if its numerical volume is zero */
Bool Mesh::isNumericallyEmpty() const
{
  return getVolume() <= ResourceMap::GetAsNumericalScalar("Mesh-SmallVolume");
}

/* Check if the given point is inside of the closed mesh */
Bool Mesh::contains(const NumericalPoint & point) const
{
  return numericallyContains(point);
}

/* Check the mesh validity */
Bool Mesh::isValid() const
{
  // Check the vertices: no duplicate, no unused vertex
  // Check the simplices: no simplex with duplicate vertices, no simplex with unknown vertex, no simplex with a number of vertices different from dimension+1
  // Check that no ball can be included into the intersection of two simplices
  return true;
}

/* Check if the given point is numerically inside of the closed mesh, i.e. using only the bounds part of the mesh */
Bool Mesh::numericallyContains(const NumericalPoint & point) const
{
  const UnsignedLong simplicesSize(simplices_.getSize());
  for (UnsignedLong i = 0; i < simplicesSize; ++i) if (checkPointInSimplex(point, i)) return true;
  return false;
}

/* Check if the given point is in the given simplex */
SquareMatrix Mesh::buildSimplexMatrix(const UnsignedLong index) const
{
  if (index >= simplices_.getSize()) throw InvalidArgumentException(HERE) << "Error: the simplex index=" << index << " must be less than the number of simplices=" << simplices_.getSize();
  SquareMatrix matrix(dimension_ + 1);
  const Indices vertexIndices(simplices_[index]);
  // Loop over the vertices of the simplex
  for (UnsignedLong j = 0; j <= dimension_; ++j)
    {
      const NumericalPoint vertexJ(vertices_[vertexIndices[j]]);
      for (UnsignedLong i = 0; i < dimension_; ++i) matrix(i, j) = vertexJ[i];
      matrix(dimension_, j) = 1.0;
    }
  return matrix;
}

/* Check if the given point is in the given simplex */
Bool Mesh::checkPointInSimplex(const NumericalPoint & point,
                               const UnsignedLong index) const
{
  SquareMatrix matrix(buildSimplexMatrix(index));
  NumericalPoint v(point);
  v.add(1.0);
  const NumericalPoint alpha(matrix.solveLinearSystem(v, false));
  for (UnsignedLong i = 0; i <= dimension_; ++i) if ((alpha[i] < 0.0) || (alpha[i] > 1.0)) return false;
  return true;
}

/* Get the numerical volume of the mesh */
NumericalScalar Mesh::getVolume() const
{
  if (!isAlreadyComputedVolume_) computeVolume();
  return volume_;
}

/* Get the numerical volume of the mesh */
NumericalScalar Mesh::getNumericalVolume() const
{
  return getVolume();
}

/* Compute the volume of a given simplex */
NumericalScalar Mesh::computeSimplexVolume(const UnsignedLong index) const
{
  SquareMatrix matrix(buildSimplexMatrix(index));
  NumericalScalar sign(0.0);
  return exp(matrix.computeLogAbsoluteDeterminant(sign, false) - SpecFunc::LogGamma(dimension_ + 1));
}

/* Compute the volume of the mesh */
void Mesh::computeVolume() const
{
  volume_ = 0.0;
  for (UnsignedLong i = 0; i < simplices_.getSize(); ++i) volume_ += computeSimplexVolume(i);
  isAlreadyComputedVolume_ = true;
}

/* Comparison operator */
Bool Mesh::operator == (const Mesh & other) const
{
  if (this == &other) return true;
  return (vertices_ == other.vertices_) && (simplices_ == other.simplices_);
}

/* String converter */
String Mesh::__repr__() const
{
  return OSS() << "class=" << GetClassName()
               << " name=" << getName()
               << " dimension=" << getDimension()
               << " vertices=" << vertices_.__repr__()
               << " simplices=" << simplices_.__repr__();
}

String Mesh::__str__(const String & offset) const
{
  return __repr__();
}

/* Drawing method */
Graph Mesh::draw() const
{
  if (dimension_ > 2) throw InvalidArgumentException(HERE) << "Error: cannot draw a mesh of dimension > 2.";
  const UnsignedLong verticesSize(vertices_.getSize());
  const UnsignedLong simplicesSize(simplices_.getSize());
  if ((verticesSize == 0) || (simplicesSize == 0)) throw InvalidArgumentException(HERE) << "Error: cannot draw a mesh with no vertex or no simplex.";
  Graph graph(String(OSS() << "Mesh " << getName()), "x", "y", true, "topright");
  if (dimension_ == 1)
    {
      // The vertices
      Cloud vertices(vertices_, NumericalSample(verticesSize, NumericalPoint(1, 0.0)));
      vertices.setColor("red");
      vertices.setLegendName(String(OSS() << verticesSize << " node" << (verticesSize > 1 ? "s" : "")));
      // The simplices
      for (UnsignedLong i = 0; i < simplicesSize; ++i)
        {
          NumericalSample data(2, 2);
          data[0][0] = vertices_[simplices_[i][0]][0];
          data[1][0] = vertices_[simplices_[i][1]][0];
          Curve simplex(data);
          simplex.setColor("blue");
          if (i == 0) simplex.setLegendName(String(OSS() << simplicesSize << " element" << (simplicesSize > 1 ? "s" : "")));
          graph.add(simplex);
        }
      graph.add(vertices);
    }
  else
    {
      // Dimension = 2
      // The vertices
      Cloud vertices(vertices_);
      vertices.setColor("red");
      vertices.setLegendName(String(OSS() << verticesSize << " node" << (verticesSize > 1 ? "s" : "")));
      // The simplices
      for (UnsignedLong i = 0; i < simplicesSize; ++i)
        {
          NumericalSample data(4, 2);
          data[0] = vertices_[simplices_[i][0]];
          data[1] = vertices_[simplices_[i][1]];
          data[2] = vertices_[simplices_[i][2]];
          data[3] = vertices_[simplices_[i][0]];
          Curve simplex(data);
          simplex.setColor("blue");
          if (i == 0) simplex.setLegendName(String(OSS() << simplicesSize << " element" << (simplicesSize > 1 ? "s" : "")));
          graph.add(simplex);
        }
      graph.add(vertices);
    }
  return graph;
}

/* Import mesh from FreeFem 2D mesh files */
Mesh Mesh::ImportFromMSHFile(const String & fileName)
{
  std::ifstream file(fileName.c_str(), std::ios::in);
  if (!file) throw FileNotFoundException(HERE) << "Error: can't open file " << fileName;
  // Bording case: empty file
  if (file.eof())
    {
      Log::Info(OSS() << "File " << fileName << " is empty.");
      return Mesh();
    }
  // First, the header: it is made of 3 integers, the number of vertices, the number of simplices and the number of elements on the boundary, currently not used by OT
  UnsignedLong verticesNumber(0);
  UnsignedLong simplicesNumber(0);
  UnsignedLong scratch(0);
  file >> verticesNumber;
  file >> simplicesNumber;
  file >> scratch;
  LOGINFO(OSS() << "Number of vertices=" << verticesNumber << ", number of simplices=" << simplicesNumber);
  // Parse the vertices
  NumericalSample vertices(verticesNumber, 2);
  for (UnsignedLong i = 0; i < verticesNumber; ++i)
    {
      file >> vertices[i][0];
      file >> vertices[i][1];
      file >> scratch;
      LOGINFO(OSS() << "vertex " << i << "=" << vertices[i]);
    }
  // Parse the simplices
  IndicesCollection simplices(simplicesNumber, Indices(3));
  for (UnsignedLong i = 0; i < simplicesNumber; ++i)
    {
      file >> simplices[i][0];
      file >> simplices[i][1];
      file >> simplices[i][2];
      --simplices[i][0];
      --simplices[i][1];
      --simplices[i][2];
      file >> scratch;
      LOGINFO(OSS() << "simplex " << i << "=" << simplices[i]);
    }
  file.close();
  return Mesh(vertices, simplices);
}

/* Method save() stores the object through the StorageManager */
void Mesh::save(Advocate & adv) const
{
  DomainImplementation::save(adv);
  adv.saveAttribute("vertices_", vertices_);
  adv.saveAttribute("simplices_", simplices_);
  adv.saveAttribute("volume_", volume_);
  adv.saveAttribute("isAlreadyComputedVolume_", isAlreadyComputedVolume_);
}

/* Method load() reloads the object from the StorageManager */
void Mesh::load(Advocate & adv)
{
  DomainImplementation::load(adv);
  adv.loadAttribute("vertices_", vertices_);
  adv.loadAttribute("simplices_", simplices_);
  adv.loadAttribute("volume_", volume_);
  adv.loadAttribute("isAlreadyComputedVolume_", isAlreadyComputedVolume_);
}

END_NAMESPACE_OPENTURNS
