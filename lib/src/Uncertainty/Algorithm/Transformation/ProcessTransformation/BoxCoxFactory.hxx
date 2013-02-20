//                                               -*- C++ -*-
/**
 *  @file  BoxCoxFactory.hxx
 *  @brief This class is enables to estimate parameters of the BoxCox model
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 *  Id      BoxCoxFactory.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_BOXCOXFACTORY_HXX
#define OPENTURNS_BOXCOXFACTORY_HXX

#include "PersistentObject.hxx"
#include "NumericalPoint.hxx"
#include "NumericalSample.hxx"
#include "TimeSeries.hxx"
#include "BoxCoxTransform.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class BoxCoxFactory
 */

class BoxCoxFactory
  : public PersistentObject
{

  CLASSNAME;

public:

  /** Default constructor without parameters */
  BoxCoxFactory(const String & name = DefaultName);

  /** Default constructor without parameters */
  BoxCoxFactory(const NumericalScalar alpha,
                const String & name = DefaultName);

  /** alpha accessors */
  NumericalScalar getAlpha() const;
  void setAlpha(const NumericalScalar alpha);

  /** Virtual copy constructor */
  BoxCoxFactory * clone() const;

  /** String converter */
  String __repr__() const;

  /** String converter */
  String __str__(const String & offset = "") const;

  /** Build the factory from data by estimating the best \lambda which maximizes the log-likelihood function */
  BoxCoxTransform * build(const TimeSeries & timeSeries) const;


protected:

  /** Likelihood function */
  NumericalScalar computeLogLikelihood(const NumericalScalar & lambda) const;

  /** wrapper function passed to cobyla */
  static int ComputeObjectiveAndConstraint( int n,
                                            int m,
                                            double * x,
                                            double * f,
                                            double * con,
                                            void * state);

  /** only used to pass data to be used in computeLogLikeliHood */
  mutable NumericalSample sample_;

  /** only used to pass data to be used in computeLogLikeliHood */
  mutable NumericalScalar scalar_;

  /** alpha is a scalar used for translating values */
  NumericalScalar alpha_;

} ; /* class BoxCoxFactory */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_BOXCOXFACTORY_HXX */
