//                                               -*- C++ -*-
/**
 *  @file  Thread.cxx
 *  @brief The class Thread realizes multithreading implementation
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
#include "Threadable.hxx"
#include "ThreadStatus.hxx"
#include "Thread.hxx"
#include "OSS.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(Thread);

/* Default constructor */
Thread::Thread(const Threadable & th ///< The Threadable object this Thread object must run
               ) : threadable_(th)
{
  // Nothing to do
}

/* Copy constructor */
Thread::Thread(const Thread & other ///< The copied Thread
               ) : threadable_(other.threadable_)
{
  // Nothing to do
}

/* Destructor */
Thread::~Thread()
{
  // Nothing to do
}

/* Assignment */
Thread & Thread::operator =(const Thread & other)
{
  if (this != &other)
    {
      // Other is NOT me, so I can assign it to me
    }
  return *this;
}

/* Comparison operator */
Bool Thread::operator ==(const Thread & other) const
{
  Bool sameThread = false;

  if (this != &other)
    {
      // Other is NOT me, so I have to realize the comparison
      // sameThread = ...
    }

  else
    sameThread = true;

  return sameThread;
}

/* String converter */
String Thread::__repr__() const
{
  return OSS() << "class=" << Thread::GetClassName();
}


/*
 * Start running the Threadable object
 *
 * @todo implement Thread::start() method
 */
void Thread::start() const
{
  // TODO
}

/*
 * Cancel execution of the Threadable object
 * Because cancellation is not abortion, it is up to the
 * Threadable to accept cancellation. So calling this method
 * does not mean that the Thread will actually stop
 *
 * @todo implement Thread::cancel() method
 */
void Thread::cancel() const
{

}

/*
 * Get a ThreadStatus object that informs the user of
 * the internal status of the current thread
 *
 * @todo implement Thread::getStatus() method
 */
const ThreadStatus Thread::getStatus() const
{
  return ThreadStatus();
}



END_NAMESPACE_OPENTURNS
