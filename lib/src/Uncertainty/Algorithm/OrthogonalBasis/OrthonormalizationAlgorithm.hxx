//                                               -*- C++ -*-
/**
 *  @file  OrthonormalizationAlgorithm.hxx
 *  @brief
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
 *  @date   2009-09-14 14:39:35 +0200 (lun. 14 sept. 2009)
 *  Id      OrthonormalizationAlgorithm.hxx 1331 2009-09-14 12:39:35Z dutka
 */
#ifndef OPENTURNS_ORTHONORMALIZATIONALGORITHM_HXX
#define OPENTURNS_ORTHONORMALIZATIONALGORITHM_HXX

#include "OrthonormalizationAlgorithmImplementation.hxx"
#include "Pointer.hxx"
#include "TypedInterfaceObject.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class OrthonormalizationAlgorithm
 */

class OrthonormalizationAlgorithm
  : public TypedInterfaceObject<OrthonormalizationAlgorithmImplementation>
{
  CLASSNAME;

public:
  typedef Pointer<OrthonormalizationAlgorithmImplementation> Implementation;
  typedef OrthonormalizationAlgorithmImplementation::Coefficients          Coefficients;


  /** Constructor with parameters */
  OrthonormalizationAlgorithm(const OrthonormalizationAlgorithmImplementation & implementation);

  /** Constructor with parameters */
  OrthonormalizationAlgorithm(Implementation & p_implementation);

  /** Constructor with parameters */
  OrthonormalizationAlgorithm(const Distribution & measure);

  /** Default Constructor */
  explicit OrthonormalizationAlgorithm();

  /** Calculate the coefficients of recurrence a0, a1, a2 such that
      Pn+1(x) = (a0 * x + a1) * Pn(x) + a2 * Pn-1(x) */
  Coefficients getRecurrenceCoefficients(const UnsignedLong n) const;

  /** Measure accessor */
  Distribution getMeasure() const;
  void setMeasure(const Distribution & measure);

  /** String converter */
  String __repr__() const;

  /** String converter */
  String __str__(const String & offset = "") const;

} ; /* class OrthonormalizationAlgorithm */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ORTHONORMALIZATIONALGORITHM_HXX */
