//                                               -*- C++ -*-
/**
 *  @file  ApproximationAlgorithmImplementation.hxx
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
 *  Id      ApproximationAlgorithmImplementation.hxx 1649 2010-11-09 12:44:00Z dutka
 */
#ifndef OPENTURNS_APPROXIMATIONALGORITHMIMPLEMENTATION_HXX
#define OPENTURNS_APPROXIMATIONALGORITHMIMPLEMENTATION_HXX

#include "PersistentObject.hxx"
#include "NumericalPoint.hxx"
#include "NumericalSample.hxx"
#include "Basis.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class ApproximationAlgorithmImplementation
 *
 * Regression from a data sample upon a particular basis
 */
class ApproximationAlgorithmImplementation
  : public PersistentObject
{
  CLASSNAME;

public:


  /** Default constructor */
  ApproximationAlgorithmImplementation();

  /** Default constructor */
  ApproximationAlgorithmImplementation(const NumericalSample & x,
                                       const NumericalSample & y,
                                       const Basis & psi);
  /** Default constructor */
  ApproximationAlgorithmImplementation(const NumericalSample & x,
                                       const NumericalSample & y,
                                       const NumericalPoint & weight,
                                       const Basis & psi);

  /** Virtual constructor */
  virtual ApproximationAlgorithmImplementation * clone() const = 0;

  /** Sample accessors */
  NumericalSample getX() const;
  NumericalSample getY() const;

  /** Weigths accessor */
  NumericalPoint getWeight() const;

  /** Basis accessor */
  Basis getPsi() const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Perform the selection */
  virtual void run() = 0;

  /** Acccessor to the coefficients of the selected metamodel on the provided basis */
  NumericalPoint getCoefficients();

  /** Acccessor to the residual of the selected metamodel on the provided basis */
  NumericalScalar getResidual();

  NumericalScalar getRelativeError();

  /** Verbosity accessor */
  void setVerbose(const Bool verbose);
  Bool getVerbose() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:

  void setCoefficients(const NumericalPoint & coefficients);

  void setResidual(const NumericalScalar residual);

  void setRelativeError(const NumericalScalar relativeError);

  /** Input sample */
  NumericalSample x_;

  /** Output sample */
  NumericalSample y_;

  /** Weights */
  NumericalPoint weight_;

  /** Basis */
  Basis psi_;

  mutable Bool isAlreadyComputedCoefficients_;

private:
  /** Regression coefficients */
  NumericalPoint coefficients_;

  /** Residual */
  NumericalScalar residual_;

  /** Relative error */
  NumericalScalar relativeError_;

  /** Verbosity flag */
  Bool verbose_;


}; /* class ApproximationAlgorithmImplementation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_APPROXIMATIONALGORITHMIMPLEMENTATION_HXX */
