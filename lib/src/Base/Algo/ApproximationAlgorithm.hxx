//                                               -*- C++ -*-
/**
 *  @file  ApproximationAlgorithm.hxx
 *  @brief Regression from a data sample upon a particular basis
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
 *  Id      ApproximationAlgorithm.hxx 1649 2010-11-09 12:44:00Z dutka
 */
#ifndef OPENTURNS_APPROXIMATIONALGORITHM_HXX
#define OPENTURNS_APPROXIMATIONALGORITHM_HXX

#include "TypedInterfaceObject.hxx"
#include "ApproximationAlgorithmImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class ApproximationAlgorithm
 *
 * Regression from a data sample upon a particular basis
 */
class ApproximationAlgorithm
  : public TypedInterfaceObject<ApproximationAlgorithmImplementation>
{
  CLASSNAME;

public:

  typedef TypedInterfaceObject<ApproximationAlgorithmImplementation>::Implementation Implementation;

  /** Constructor from implementation */
  ApproximationAlgorithm(const ApproximationAlgorithmImplementation & implementation);

  /** Constructor from implementation */
  ApproximationAlgorithm(const Implementation & p_implementation);

  /** Sample accessors */
  NumericalSample getX() const;
  NumericalSample getY() const;

  /** Weights accessors */
  NumericalPoint getWeight() const;

  /** Basis accessor */
  Basis getPsi() const;

  /** Verbosity accessor */
  void setVerbose(const Bool verbose);
  Bool getVerbose() const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** Perform approximation */
  void run();

  /** Acccessor to the coefficients of the selected metamodel on the provided basis */
  NumericalPoint getCoefficients();

  /** Acccessor to the residual of the selected metamodel on the provided basis */
  NumericalScalar getResidual();

  /** Acccessor to the residual of the selected metamodel on the provided basis */
  NumericalScalar getRelativeError();

}; /* class ApproximationAlgorithm */



END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_APPROXIMATIONALGORITHM_HXX */