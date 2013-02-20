//                                               -*- C++ -*-
/**
 *  @file  LinearModel.hxx
 *  @brief LinearModel implements the linear model
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
 *  Id      LinearModel.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_LINEARMODEL_HXX
#define OPENTURNS_LINEARMODEL_HXX

#include "PersistentObject.hxx"
#include "Description.hxx"
#include "NumericalPoint.hxx"
#include "NumericalSample.hxx"
#include "ConfidenceInterval.hxx"
#include "PersistentCollection.hxx"
#include "Collection.hxx"
#include "TestResult.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class LinearModel
 *
 * LinearModel implements the notion of linear model
 */

class LinearModel :
  public PersistentObject
{
  CLASSNAME;

public:

  typedef Collection<ConfidenceInterval>           ConfidenceIntervalCollection;
  typedef PersistentCollection<ConfidenceInterval> ConfidenceIntervalPersistentCollection;
  typedef Collection<NumericalScalar>                    NumericalScalarCollection;
  typedef PersistentCollection<NumericalScalar>          NumericalScalarPersistentCollection;


  /** Default constructor is private */
  LinearModel();

  /** Standard constructor */
  LinearModel(const NumericalPoint & vectorR,
              const ConfidenceIntervalCollection & intervalsColl,
              const NumericalScalarCollection & pValuesOfR);

  /** Constructor from NumericalPoint */
  LinearModel(const NumericalPoint & vectorR);

  /** Virtual constructor */
  virtual LinearModel * clone() const;


  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** get vector, get intervals*/
  const NumericalPoint & getRegression() const;
  const ConfidenceIntervalCollection & getConfidenceIntervals() const;
  const NumericalScalarCollection &getPValues() const;

  /** getPredicted : build an sample of predicted values */
  NumericalSample getPredicted(const NumericalSample & predictor) const;

  /** getResidual */
  NumericalSample getResidual(const NumericalSample & predictor,
                              const NumericalSample & measured) const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);


protected:

  /** The estimated vector */
  NumericalPoint regression_;

  /** The confidence intervals */
  ConfidenceIntervalPersistentCollection confidenceIntervals_;

  /** The p-Values  */
  NumericalScalarPersistentCollection pValues_;


private:

  friend class Factory<LinearModel>;

}; /* class LinearModel */


/** Comparison operator */
Bool operator ==(const LinearModel & lhs, const LinearModel & rhs);

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LINEARMODEL_HXX */
