//                                               -*- C++ -*-
/**
 *  @file  Mvndstpack.hxx
 *  @brief This file includes all of the mvndstpack functions used in the platform
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
 *  @author schueller
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 *  Id      Mvndstpack.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_MVNDSTPACK_HXX
#define OPENTURNS_MVNDSTPACK_HXX

#include "OTconfig.hxx"
#ifdef HAVE_FORTRANCINTERFACE
#  include "FortranCInterface.h"
#endif /* HAVE_FORTRANCINTERFACE */

BEGIN_C_DECLS

/** Function that evaluates n-dimensional normal CDF */
#define MVNDST_F77 F77_FUNC(mvndst,MVNDST)
void MVNDST_F77(int *n, double *lower, double *upper, int *infin, double *correl, int *maxpts, double *abseps, double *releps, double *error, double *value, int *inform);

END_C_DECLS

#endif /* OPENTURNS_MVNDSTPACK_HXX */
