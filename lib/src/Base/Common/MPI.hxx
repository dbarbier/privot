//                                               -*- C++ -*-
/**
 *  @file  MPI.hxx
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
 *  Id      AtomicFunctions.hxx 434 2007-05-10 14:43:31Z dutka
 */
#ifndef OPENTURNS_MPI_HXX
#define OPENTURNS_MPI_HXX

#include "OTprivate.hxx"
#include "OTconfig.hxx"
#include "AtomicFunctions.hxx"
#include "Pointer.hxx"

#ifdef HAVE_MPI
#  include "mpi.h"
#endif /* HAVE_MPI */

BEGIN_NAMESPACE_OPENTURNS

/**
 * class MPICommunicator
 *
 * brief This class manages a MPI communicator between parallel instances.
 */

class MPICommunicator
{
  friend class MPI;

public:
#ifdef HAVE_MPI
  typedef MPI_Comm CommunicatorType;
#else
  typedef int      CommunicatorType;
#endif /* HAVE_MPI */


  MPICommunicator(const CommunicatorType & communicator);
  MPICommunicator(const MPICommunicator & other);
  ~MPICommunicator();

  /** Return a global communicator that encompass all compute nodes */
  static MPICommunicator GetCommWorld();

private:

  Pointer<CommunicatorType> p_comm_;

}; // end class MPICommunicator



/**
 * class MPI
 *
 * brief This class is a singleton that initializes the MPI universe.
 */

class MPI
{
  /** The communicator used to send and receive message between instances */
  MPICommunicator comm_;

  /** Constructor */
  MPI(const MPICommunicator & comm);

public:

  ~MPI() throw();

  /** Start the machinery of message-passing */
  static void Init(const MPICommunicator & comm);

  /** End the machinery of message-passing */
  static void Release();

}; // end class MPI

END_NAMESPACE_OPENTURNS


#endif /* OPENTURNS_MPI_HXX */
