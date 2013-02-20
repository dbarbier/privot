//                                               -*- C++ -*-
/**
 *  @file  TBB.cxx
 *  @brief This file supplies support for multithreading
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
 *  @date   2010-01-27 14:00:03 +0100 (Wed, 27 Jan 2010)
 */

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <errno.h>

#include "OTconfig.hxx"
#include "OTthread.hxx"

// Those two includes must be calles in this order: ResourceMap before TBB. See #400.
#include "ResourceMap.hxx"
#include "TBB.hxx"

#include "MutexLock.hxx"
#include "Log.hxx"

BEGIN_NAMESPACE_OPENTURNS

static pthread_mutex_t TBB_InstanceMutex_;
static TBB * TBB_P_instance_ = 0;
static pthread_once_t TBB_InstanceMutex_once = PTHREAD_ONCE_INIT;

#ifdef HAVE_TBB
tbb::task_scheduler_init * TBB_P_scheduler_ = 0;
#endif /* HAVE_TBB */


TBB_init::TBB_init()
{
  int rc = pthread_once( &TBB_InstanceMutex_once, TBB::Initialization );
  if (rc != 0)
    {
      perror("TBB_init::TBB_init once Initialization failed");
      exit(1);
    }
}

TBB_init::~TBB_init()
{
  TBB::Release();
}


void TBB::Initialization()
{
  pthread_mutexattr_t attr;
  pthread_mutexattr_init( &attr );
  //pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_NORMAL );
  pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_ERRORCHECK );
  //pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );
  int rc = pthread_mutex_init( &TBB_InstanceMutex_, &attr );
  if (rc != 0)
    {
      perror("TBB::Initialization mutex initialization failed");
      exit(1);
    }
#ifdef BOGUS_PTHREAD_LIBRARY
  TBB_P_instance_ = 0;
#else
  TBB_P_instance_ = new TBB;
#ifdef HAVE_TBB
  UnsignedLong nbThreads = ResourceMap::GetAsUnsignedLong( "parallel-threads" );
  TBB_P_scheduler_ = new tbb::task_scheduler_init( nbThreads );
#endif /* HAVE_TBB */
#endif /* BOGUS_PTHREAD_LIBRARY */
  // std::atexit( TBB::Release );
}


void TBB::Release()
{
  delete TBB_P_instance_;
  TBB_P_instance_ = 0;
#ifdef HAVE_TBB
  delete TBB_P_scheduler_;
  TBB_P_scheduler_ = 0;
#endif /* HAVE_TBB */
}


END_NAMESPACE_OPENTURNS
