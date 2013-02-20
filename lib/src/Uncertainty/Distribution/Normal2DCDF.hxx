//                                               -*- C++ -*-
/**
 *  @file  Normal2DCDF.hxx
 *  @brief Efficient implementation of the CDF computation for a bi-dimensional
 *         Normal distribution
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
 *  @date   2012-07-16 12:24:33 +0200 (Mon, 16 Jul 2012)
 *  Id      Normal2DCDF.hxx 2604 2012-07-16 10:24:33Z schueller
 */
#ifndef OPENTURNS_NORMALCDF2D_HXX
#define OPENTURNS_NORMALCDF2D_HXX

#include "OTprivate.hxx"

BEGIN_NAMESPACE_OPENTURNS

NumericalScalar Normal2DCDF(const NumericalScalar x1,
                            const NumericalScalar x2,
                            const NumericalScalar rho,
                            const Bool tail);

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_NORMALCDF2D_HXX */
