//                                               -*- C++ -*-
/**
 *  @file  TrapezoidalFactory.hxx
 *  @brief Factory for Trapezoidal distribution
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
 *  @date   2009-09-30 17:51:57 +0200 (mer. 30 sept. 2009)
 *  Id      TrapezoidalFactory.hxx 1357 2009-09-30 15:51:57Z schueller
 */
#ifndef OPENTURNS_TRAPEZOIDALFACTORY_HXX
#define OPENTURNS_TRAPEZOIDALFACTORY_HXX

#include "OTprivate.hxx"
#include "DistributionImplementationFactory.hxx"
#include "Trapezoidal.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class TrapezoidalFactory
 */
class TrapezoidalFactory
  : public DistributionImplementationFactory
{
  CLASSNAME;
public:

  /** Default constructor */
  TrapezoidalFactory();

  /** Virtual constructor */
  TrapezoidalFactory * clone() const;

  /* Here is the interface that all derived class must implement */
  using DistributionImplementationFactory::build;

  Implementation build( const NumericalSample & sample ) const;
  Implementation build( const NumericalPointCollection & parameters ) const;
  Implementation build() const;
  Trapezoidal buildAsTrapezoidal( const NumericalSample & sample ) const;
  Trapezoidal buildAsTrapezoidal( const NumericalPointCollection & parameters ) const;
  Trapezoidal buildAsTrapezoidal( const NumericalPointWithDescriptionCollection & parameters ) const;
  Trapezoidal buildAsTrapezoidal() const;

protected:

  /** likelihood estimate */
  NumericalScalar computeLogLikelihood( const NumericalPoint & x ) const;

  /** wrapper function passed to cobyla */
  static int ComputeObjectiveAndConstraint( int n,
                                            int m,
                                            double * x,
                                            double * f,
                                            double * con,
                                            void * state );

  /** only used to pass data to be used in computeLogLikeliHood */
  mutable NumericalSample sample_;

}; /* class TrapezoidalFactory */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_TRAPEZOIDALFACTORY_HXX */
