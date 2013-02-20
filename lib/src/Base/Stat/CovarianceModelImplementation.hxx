//                                               -*- C++ -*-
/**
 *  @file  CovarianceModelImplementation.hxx
 *  @brief This class enables to build a covariance model
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
 *  @author lebrun
 *  @date   2011-12-05 17:31:12 +0100 (lun. 05 déc. 2011)
 *  Id      CovarianceModelImplementation.hxx 2226 2011-12-05 16:31:12Z lebrun
 */
#ifndef OPENTURNS_COVARIANCEMODELIMPLEMENTATION_HXX
#define OPENTURNS_COVARIANCEMODELIMPLEMENTATION_HXX

#include "PersistentObject.hxx"
#include "CovarianceMatrix.hxx"
#include "Pointer.hxx"
#include "TimeSeriesImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class CovarianceModelImplementation
 */

class CovarianceModelImplementation
  : public PersistentObject
{

  CLASSNAME;

public:

  typedef Pointer<CovarianceModelImplementation>    Implementation;

  /** Default constructor without parameters */
  CovarianceModelImplementation(const String & name = DefaultName);

  /** Constructor with dimension parameter */
  CovarianceModelImplementation(const UnsignedLong dimension,
                                const String & name = DefaultName);

  /** Virtual copy constructor */
  virtual CovarianceModelImplementation * clone() const;

  /** Dimension accessor */
  virtual UnsignedLong getDimension() const;

  /** Computation of the covariance matrix */
  virtual CovarianceMatrix computeCovariance(const NumericalScalar s,
                                             const NumericalScalar t) const;

  /** Computation of the covariance matrix for stationnary models */
  virtual CovarianceMatrix computeCovariance(const NumericalScalar tau) const;

  /** Discretize the covariance function on a given TimeGrid */
  virtual CovarianceMatrix discretizeCovariance(const RegularGrid & timeGrid) const;

  /** Is it a stationary covariance model ? */
  virtual Bool isStationary() const;

  /** String converter */
  virtual String __repr__() const;

  /** String converter */
  virtual String __str__(const String & offset = "") const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:

  /** dimension parameter */
  UnsignedLong dimension_;

} ; /* class CovarianceModelImplementation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_COVARIANCEMODELIMPLEMENTATION_HXX */
