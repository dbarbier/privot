//                                               -*- C++ -*-
/**
 *  @file  AtomicFunctions.cxx
 *  @brief This file supplies some atomic functions to support multithreading
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
 *  @date   2007-05-10 16:43:31 +0200 (Thu, 10 May 2007)
 */

#include "AtomicFunctions.hxx"

BEGIN_NAMESPACE_OPENTURNS

#if !defined(HAVE_SYNC_PRIMITIVES)
pthread_mutex_t Atomic::Atomic_Mutex_ = PTHREAD_MUTEX_INITIALIZER;
#endif

#if !defined(HAVE_SYNC_PRIMITIVES) && !defined(WIN32) && !defined(__i386__)
#warning "Using slow implementation for synchronization functions."
#endif

END_NAMESPACE_OPENTURNS

