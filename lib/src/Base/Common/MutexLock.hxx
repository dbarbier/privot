//                                               -*- C++ -*-
/**
 *  @file  MutexLock.hxx
 *  @brief The class MutexLock manages the lock/unlock of Pthread mutexes
 *         This file is intended to be only include in .cxx files (avoid .hxx)
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
 *  @date   2010-02-04 16:44:49 +0100 (Thu, 04 Feb 2010)
 *  Id      MutexLock.hxx 1473 2010-02-04 15:44:49Z dutka
 */
#ifndef OPENTURNS_MUTEXLOCK_HXX
#define OPENTURNS_MUTEXLOCK_HXX

#include <errno.h>  // for errno(3)
#include <stdio.h>  // for perror(3)
#include <stdlib.h> // for exit(3)
#include <string.h> // for strerror(3)
#include "OTprivate.hxx"
#include "OTthread.hxx"

BEGIN_NAMESPACE_OPENTURNS

class MutexLock
{
  pthread_mutex_t & mtx_;
public:
  MutexLock( pthread_mutex_t & mtx )
    : mtx_(mtx)
  {
    int rc = pthread_mutex_lock( &mtx_ );
    if (rc != 0)
      {
        fprintf( stderr, "(MutexLock ctor) rc=%d : %s\n", rc, strerror(rc) );
        exit(1);
      }
  }

  MutexLock( const MutexLock & other )
    : mtx_(other.mtx_)
  {
    int rc = pthread_mutex_lock( &mtx_ );
    if (rc != 0)
      {
        fprintf( stderr, "(MutexLock copy ctor) rc=%d : %s\n", rc, strerror(rc) );
        exit(1);
      }
  }

  virtual ~MutexLock() throw()
  {
    int rc = pthread_mutex_unlock( &mtx_ );
    if (rc != 0)
      {
        fprintf( stderr, "(MutexLock dtor) rc=%d : %s\n", rc, strerror(rc) );
        exit(1);
      }
  }

}; /* class MutexLock */

END_NAMESPACE_OPENTURNS


#endif /* OPENTURNS_MUTEXLOCK_HXX */
