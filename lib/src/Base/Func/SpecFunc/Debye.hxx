//                                               -*- C++ -*-
/**
 *  @file  Debye.hxx
 *  @brief This file includes the interface to the routine that computes Debye function up to order 20.
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
 *  @date   2007-11-04 23:24:23 +0100 (dim, 04 nov 2007)
 *  Id      Debye.hxx 613 2007-11-04 22:24:23Z lebrun
 */
#ifndef OPENTURNS_DEBYE_HXX
#define OPENTURNS_DEBYE_HXX

/** Method that evaluates Debye function up to order 20. */
double debyen(const double x, const int n);

#endif /* OPENTURNS_DEBYE_HXX */
