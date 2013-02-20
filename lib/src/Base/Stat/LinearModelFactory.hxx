//                                               -*- C++ -*-
/**
 *  @file  LinearModelFactory.hxx
 *  @brief Factory for linear model
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
 *  Id      LinearModelFactory.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_LINEARMODELFACTORY_HXX
#define OPENTURNS_LINEARMODELFACTORY_HXX

#include "LinearModel.hxx"
#include "NumericalSample.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class LinearModelFactory
 *
 * Factory for linear model
 */

class LinearModelFactory
{
public:

  typedef PersistentCollection<ConfidenceInterval> ConfidenceIntervalPersistentCollection;
  typedef PersistentCollection<NumericalScalar>    NumericalScalarPersistentCollection;

  /** Default constructor */
  LinearModelFactory();

  /** LinearModel creation */
  LinearModel build(const NumericalSample & samplePred,
                    const NumericalSample & sampleLab,
                    const NumericalScalar levelValue = ResourceMap::GetAsNumericalScalar( "LinearModelFactory-DefaultLevelValue" )) const;

}; /* class LinearModelFactory */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LINEARMODELFACTORY_HXX */
