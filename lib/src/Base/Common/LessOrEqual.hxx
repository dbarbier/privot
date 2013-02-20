//                                               -*- C++ -*-
/**
 *  @file  LessOrEqual.hxx
 *  @brief Implementation of less or equal comparison
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
 *  Id      LessOrEqual.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_LESSOREQUAL_HXX
#define OPENTURNS_LESSOREQUAL_HXX

#include "ComparisonOperatorImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class LessOrEqual
 *
 * @brief The class implements the less or equal comparison
 *
 * This class is derived from ComparisonOperatorImplementation. It performs the comparison
 * of two NumericalScalar and is mainly used in threshold exceedence algorithms.
 */
class LessOrEqual
  : public ComparisonOperatorImplementation
{
  CLASSNAME;
public:

  /** Default constructor */
  explicit LessOrEqual();

  /* String converter */
  String __repr__() const;


  /* Here is the interface that all derived class must implement */

  /* Virtual constructor */
  virtual LessOrEqual * clone() const;

  /**
   * Return true if a <= b
   * @param a The left hand side of the expression
   * @param b The right hand side of the expression
   * @return True if comparison succeeds
   */
  virtual Bool operator() (NumericalScalar a, NumericalScalar b) const;

  /* Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /* Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

}; /* class LessOrEqual */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LESSOREQUAL_HXX */
