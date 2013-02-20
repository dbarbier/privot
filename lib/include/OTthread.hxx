//                                               -*- C++ -*-
/**
 *  @file  OTthread.hxx
 *  @brief The header file that declares thread operators
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
 *  @author dutka
 *  @date   2009-01-16 13:58:07 +0100 (Fri, 16 Jan 2009)
 *  Id      OTtypes.hxx 1068 2009-01-16 12:58:07Z dutka
 */
#ifndef OPENTURNS_OTTHREAD_HXX
#define OPENTURNS_OTTHREAD_HXX

#include "OTconfig.hxx"

#ifdef HAVE_PTHREAD_H

#include <pthread.h>

// MinGW pthread.h includes windows.h which defines a HUGE number of macros...
#ifdef WIN32
#undef GetClassName
#undef ERROR
#undef IN
#undef OUT
#endif

#endif /* HAVE_PTHREAD_H */

#endif /* OPENTURNS_OTTHREAD_HXX */
