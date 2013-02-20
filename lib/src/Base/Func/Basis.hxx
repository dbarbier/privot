//                                               -*- C++ -*-
/**
 *  @file  Basis.hxx
 *  @brief A collection of numerical math function
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
 *  @date   2010-11-09 13:44:00 +0100 (Tue, 09 Nov 2010)
 *  Id      Basis.hxx 1649 2010-11-09 12:44:00Z dutka
 */
#ifndef OPENTURNS_BASIS_HXX
#define OPENTURNS_BASIS_HXX

#include "PersistentCollection.hxx"
#include "NumericalMathFunction.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class Basis
 *
 * A collection of numerical math function.
 */
class Basis
  : public PersistentCollection<NumericalMathFunction>
{
  CLASSNAME;

public:

  /** Default constructor */
  Basis();

  /** Constructor from a collection */
  Basis(const Collection<NumericalMathFunction> & coll);

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Dimension accessor */
  UnsignedLong getDimension() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

}; /* class Basis */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_BASIS_HXX */
