//                                               -*- C++ -*-
/**
 *  @file  ComparisonOperatorImplementation.hxx
 *  @brief Abstract top-level class for all ComparisonOperatorImplementation
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 *  Id      ComparisonOperatorImplementation.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_COMPARISONOPERATORIMPLEMENTATION_HXX
#define OPENTURNS_COMPARISONOPERATORIMPLEMENTATION_HXX

#include "OTprivate.hxx"
#include "PersistentObject.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class ComparisonOperatorImplementation
 *
 * @brief The class implements the concept of comparison operator defined in ComparisonOperator.
 *
 * This class is abstract so it can not be instanciated. It must be derived.
 * @see ComparisonOperator
 */
class ComparisonOperatorImplementation
  : public PersistentObject
{
  CLASSNAME;
public:

  /** Default constructor */
  ComparisonOperatorImplementation();

  /* String converter */
  String __repr__() const;


  /* Here is the interface that all derived class must implement */

  /* Virtual constructor */
  virtual ComparisonOperatorImplementation * clone() const = 0;

  /** @copydoc ComparisonOperator::operator()() const */
  virtual Bool operator() (NumericalScalar a, NumericalScalar b) const = 0;

  /* Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /* Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

}; /* class ComparisonOperatorImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_COMPARISONOPERATORIMPLEMENTATION_HXX */
