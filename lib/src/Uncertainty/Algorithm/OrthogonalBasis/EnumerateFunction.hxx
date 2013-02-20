//                                               -*- C++ -*-
/**
 *  @file  EnumerateFunction.hxx
 *  @brief The bijective function to select polynomials in the orthogonal basis
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
 *  @author dutka
 *  @date   2008-05-21 17:44:02 +0200 (Wed, 21 May 2008)
 *  Id      Object.hxx 818 2008-05-21 15:44:02Z dutka
 */
#ifndef OPENTURNS_ENUMERATEFUNCTION_HXX
#define OPENTURNS_ENUMERATEFUNCTION_HXX

#include "EnumerateFunctionImplementation.hxx"
#include "Pointer.hxx"
#include "TypedInterfaceObject.hxx"
#include "NumericalPoint.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class EnumerateFunction
 *
 * The bijective function to select polynomials in the orthogonal basis
 */
class EnumerateFunction
  : public TypedInterfaceObject<EnumerateFunctionImplementation>
{
  CLASSNAME;
public:

  typedef Pointer<EnumerateFunctionImplementation> Implementation;

  /** Constructor with parameters */
  EnumerateFunction(const EnumerateFunctionImplementation & implementation);

  /** Constructor with parameters */
  EnumerateFunction(Implementation & p_implementation);

  /** Parameter constructor */
  EnumerateFunction(const UnsignedLong dimension = 1);

  /** Parameter constructor */
  EnumerateFunction(const UnsignedLong dimension,
                    const NumericalScalar q);

  /** Parameter constructor */
  EnumerateFunction(const NumericalPoint & weight,
                    const NumericalScalar q);

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** The bijective association between an integer and a set of indices */
  Indices operator() (const UnsignedLong index) const;

  /** The inverse of the association */
  UnsignedLong inverse(const Indices & indices) const;

  /** The cardinal of the given strata */
  UnsignedLong getStrataCardinal(const UnsignedLong strataIndex) const;

  /** The cardinal of the cumulated strata above or equal to the given strata */
  UnsignedLong getStrataCumulatedCardinal(const UnsignedLong strataIndex) const;

  /** Dimension accessor */
  void setDimension(const UnsignedLong dimension);
  UnsignedLong getDimension() const;

protected:

private:


} ; /* class EnumerateFunction */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ENUMERATEFUNCTION_HXX */
