//                                               -*- C++ -*-
/**
 *  @file  IdFactory.cxx
 *  @brief The class IdFactory builds unique Ids for all Objects
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
 */
#include <cstdlib>
#include "IdFactory.hxx"
#include "OSS.hxx"
#include "OTthread.hxx"
#include "MutexLock.hxx"

BEGIN_NAMESPACE_OPENTURNS



static pthread_once_t IdFactory_InstanceMutex_once = PTHREAD_ONCE_INIT;
static AtomicInt IdFactory_NextId_;


static void IdFactory_Initialization()
{
  // Nothing to do
}


IdFactory_init::IdFactory_init()
{
  int rc = pthread_once( &IdFactory_InstanceMutex_once, IdFactory_Initialization );
  if (rc != 0)
    {
      perror("IdFactory_init::IdFactory_init once Initialization failed");
      exit(1);
    }
}


/* Default constructor */
IdFactory::IdFactory()
{
  // Nothing to do
}


/* Id accessor */
Id IdFactory::BuildId()
{
  return IdFactory_NextId_.fetchAndAdd( 1 );
}



END_NAMESPACE_OPENTURNS
