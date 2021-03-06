//                                               -*- C++ -*-
/**
 *  @file  Less.hxx
 *  @brief Implementation of strict less comparison
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
 *  Id      Less.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_LESS_HXX
#define OPENTURNS_LESS_HXX

#include "ComparisonOperatorImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class Less
 *
 * @brief The class implements the strict less comparison
 *
 * This class is derived from ComparisonOperatorImplementation. It performs the comparison
 * of two NumericalScalar and is mainly used in threshold exceedence algorithms.
 */
class Less
  : public ComparisonOperatorImplementation
{
  CLASSNAME;
public:

  /** Default constructor */
  explicit Less();

  /* String converter */
  String __repr__() const;


  /* Here is the interface that all derived class must implement */

  /* Virtual constructor */
  virtual Less * clone() const;

  /** @copydoc ComparisonOperator::operator()(NumericalScalar,NumericalScalar) const
   * Return true if a < b
   */
  virtual Bool operator() (NumericalScalar a, NumericalScalar b) const;

  /* Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /* Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

}; /* class Less */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LESS_HXX */
