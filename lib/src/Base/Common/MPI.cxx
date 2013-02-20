//                                               -*- C++ -*-
/**
 *  @file  MPI.cxx
 *  @brief This file supplies support for message-passing parallelism
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
#include <pthread.h>
#include "Log.hxx"
#include "MPI.hxx"

BEGIN_NAMESPACE_OPENTURNS

static pthread_once_t MPI_init_once_control = PTHREAD_ONCE_INIT;
static pthread_once_t MPI_fina_once_control = PTHREAD_ONCE_INIT;

/* Perform the actual initialization of MPI universe */
static inline
void Initialization()
{
#ifdef HAVE_MPI
  int isInitialized = 0;
  MPI_Initialized( &isInitialized );
  if (! isInitialized ) MPI_Init( 0, 0 );
#endif
}

/* Perform the actual finalization of MPI universe */
static inline
void Finalization()
{
#ifdef HAVE_MPI
  int isFinalized = 0;
  MPI_Finalized( &isFinalized );
  if (! isFinalized ) MPI_Finalize();
#endif
}

static inline
void MPIInitialization()
{
  pthread_once( &MPI_init_once_control, Initialization );
}

static inline
void MPIFinalization()
{
  pthread_once( &MPI_fina_once_control, Finalization );
}


MPICommunicator::MPICommunicator(const CommunicatorType & communicator)
  : p_comm_(new CommunicatorType)
{
  MPIInitialization();
#ifdef HAVE_MPI
  MPI_Comm_dup( communicator, p_comm_.get() );
#endif
}

MPICommunicator::MPICommunicator(const MPICommunicator & other)
  : p_comm_(new CommunicatorType)
{
  MPIInitialization();
#ifdef HAVE_MPI
  MPI_Comm_dup( *(other.p_comm_.get()), p_comm_.get() );
#endif
}

MPICommunicator::~MPICommunicator()
{
#ifdef HAVE_MPI
  MPI_Comm_free( p_comm_.get() );
#endif
}

/* Return a global communicator that encompass all compute nodes */
MPICommunicator MPICommunicator::GetCommWorld()
{
  MPIInitialization();
#ifdef HAVE_MPI
  return MPICommunicator( MPI_COMM_WORLD );
#else
  return MPICommunicator( 0 );
#endif
}





static MPI * MPI_P_instance_ = NULL;

/* Constructor */
MPI::MPI(const MPICommunicator & comm)
  : comm_(comm)
{
  // Nothing to do
}

MPI::~MPI() throw()
{
  // Nothing to do
}

/* Start the machinery of message-passing */
void MPI::Init(const MPICommunicator & comm)
{
  MPIInitialization();
  if (! MPI_P_instance_) MPI_P_instance_ = new MPI(comm);
  assert(MPI_P_instance_);
}

/* End the machinery of message-passing */
void MPI::Release()
{
  delete MPI_P_instance_;
  MPI_P_instance_ = 0;
  MPIFinalization();
}


END_NAMESPACE_OPENTURNS
