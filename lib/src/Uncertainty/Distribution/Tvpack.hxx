//                                               -*- C++ -*-
/**
 *  @file  Tvpack.hxx
 *  @brief This file includes all of the tvpack functions used in the platform
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
 *  Id      Tvpack.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_TVPACK_HXX
#define OPENTURNS_TVPACK_HXX

#include "OTprivate.hxx"
#ifdef HAVE_FORTRANCINTERFACE
#  include "FortranCInterface.h"
#endif /* HAVE_FORTRANCINTERFACE */

BEGIN_C_DECLS

/** Function that evaluates 2-dimensional normal and student CDF. Nu=0 for normal CDF */
#define BVTL_F77 F77_FUNC(bvtl,BVTL)
double BVTL_F77(int *nu, double *dh, double *dk, double *r);

/** Function that evaluates 3-dimensional normal and student CDF. Nu=0 for normal CDF */
#define TVTL_F77 F77_FUNC(tvtl,TVTL)
double TVTL_F77(int *nu, double *h, double *r, double *epsi);

END_C_DECLS

#endif /* OPENTURNS_TVPACK_HXX */
