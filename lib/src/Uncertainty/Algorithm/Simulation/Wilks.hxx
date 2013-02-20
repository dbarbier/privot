//                                               -*- C++ -*-
/**
 *  @file  Wilks.hxx
 *  @brief Wilks is a static class that gives access to the computation of the sample size for guaranteed upper bound of quantile
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
 *  Id      Wilks.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_WILKS_HXX
#define OPENTURNS_WILKS_HXX

#include "NumericalPoint.hxx"
#include "RandomVector.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class Wilks
 */

class Wilks
{

public:


  /* Default constuctor */
  Wilks(const RandomVector & vector);

  /** Maximum sample size accessor */
  static UnsignedLong ComputeSampleSize(const NumericalScalar quantileLevel,
                                        const NumericalScalar confidenceLevel,
                                        const UnsignedLong marginIndex = 0);

  /* Estimate an upper bound of the quantile of the random vector for the given quantile level and confidence level, using the marginIndex upper statistics */
  NumericalPoint computeQuantileBound(const NumericalScalar quantileLevel,
                                      const NumericalScalar confidenceLevel,
                                      const UnsignedLong marginIndex = 0) const;

private:
  /* Random vector for which one want to compute a quantile */
  RandomVector vector_;

} ; /* class Wilks */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_WILKS_HXX */
