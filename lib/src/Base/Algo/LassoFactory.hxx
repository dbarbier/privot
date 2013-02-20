//                                               -*- C++ -*-
/**
 *  @file  LassoFactory.hxx
 *  @brief A factory for building Lasso objects
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
 *  Id      LassoFactory.hxx 1649 2010-11-09 12:44:00Z dutka
 */
#ifndef OPENTURNS_LASSOFACTORY_HXX
#define OPENTURNS_LASSOFACTORY_HXX

#include "NumericalPoint.hxx"
#include "NumericalSample.hxx"
#include "Basis.hxx"
#include "ApproximationAlgorithmImplementationFactory.hxx"
#include "Lasso.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class LassoFactory
 *
 * A factory for building Lasso objects
 */
class LassoFactory
  : public ApproximationAlgorithmImplementationFactory
{
public:


  /** Default constructor */
  LassoFactory();

  /** Virtual constructor */
  LassoFactory * clone() const;

  /** Method to create new ApproximationAlgorithmImplementation objects */
  virtual Lasso * build(const NumericalSample & x,
                        const NumericalSample & y,
                        const NumericalPoint & weight,
                        const Basis & psi) const;

  /** String converter */
  virtual String __repr__() const;

}; /* class LassoFactory */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LASSOFACTORY_HXX */
