//                                               -*- C++ -*-
/**
 *  @file  ComparisonOperator.hxx
 *  @brief Abstract top-level class for all ComparisonOperator
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
 *  Id      ComparisonOperator.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_COMPARISONOPERATOR_HXX
#define OPENTURNS_COMPARISONOPERATOR_HXX

#include "TypedInterfaceObject.hxx"
#include "ComparisonOperatorImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class ComparisonOperator
 *
 * @brief The class describes the concept of a comparison operator.
 *
 * The implementation defined what comparison is actually performed.
 * @see ComparisonOperatorImplementation
 */
class ComparisonOperator
  : public TypedInterfaceObject<ComparisonOperatorImplementation>
{
  CLASSNAME;
public:

  /** Default constructor */
  ComparisonOperator();

  /** Constructor from implementation */
  ComparisonOperator(const ComparisonOperatorImplementation & op);


  /**
   * Return true if comparison succeeds
   * @param a The left hand side of the expression
   * @param b The right hand side of the expression
   * @return True if comparison succeeds
   */
  Bool operator()(NumericalScalar a, NumericalScalar b) const;

  /** @copydoc operator()()
   * This method is an alias for ComparisonOperator::operator()(NumericalScalar a, NumericalScalar b).
   */
  Bool compare(NumericalScalar a, NumericalScalar b) const;


}; /* class ComparisonOperator */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_COMPARISONOPERATOR_HXX */
