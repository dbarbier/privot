//                                               -*- C++ -*-
/**
 *  @file  CombinatorialGenerator.hxx
 *  @brief Abstract top-level view of an combinatorialGenerator plane
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
 *  Id      CombinatorialGenerator.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_COMBINATORIALGENERATOR_HXX
#define OPENTURNS_COMBINATORIALGENERATOR_HXX

#include "TypedInterfaceObject.hxx"
#include "CombinatorialGeneratorImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class CombinatorialGenerator
 *
 * The class describes the probabilistic concept of combinatorialGenerator plan
 */
class CombinatorialGenerator
  : public TypedInterfaceObject<CombinatorialGeneratorImplementation>
{
  CLASSNAME;
public:

  typedef CombinatorialGeneratorImplementation::IndicesCollection IndicesCollection;

  /** Default constructor */
  explicit CombinatorialGenerator();

  /** Constructor from implementation */
  CombinatorialGenerator(const CombinatorialGeneratorImplementation & implementation);

#ifndef SWIG
  /** Constructor from implementation pointer */
  CombinatorialGenerator(const Implementation & p_implementation);
#endif

  /** String converter */
  virtual String __repr__() const;

  /** Sample generation */
  virtual IndicesCollection generate();

  /** Implementation accessor */
  void setImplementation(const Implementation & p_implementation);

}; /* class CombinatorialGenerator */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_COMBINATORIALGENERATOR_HXX */
