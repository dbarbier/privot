//                                               -*- C++ -*-
/**
 *  @file  Thread.hxx
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
 *  Id      Thread.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_THREAD_HXX
#define OPENTURNS_THREAD_HXX

#include "OTprivate.hxx"
#include "Threadable.hxx"
#include "ThreadStatus.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class Thread
 * @brief Implements some multithreading mechanisms
 * @internal
 *
 * Class Thread implements the multithreading mechanims for
 * the Open TURNS platform. It ensures concurrency and
 * parallelism between Threadable objects. However the
 * responsibility of shared data protection is devoted to
 * each Threadable objects. Thread class does NOT provide
 * any help in protecting those shared data from race condition
 * or any other parallel issue.
 *
 * @note This class is NOT intended to be derived. So DO NOT try
 * to do it.
 */

class Thread
{
  CLASSNAME;
public:

  /**
   * Default constructor
   */
  explicit Thread(const Threadable & th);

  /** Destructor */
  virtual ~Thread();

  /** Comparison operator */
  Bool operator ==(const Thread & other) const;

  /** @copydoc Object::__repr__() const */
  String __repr__() const;

  /**
   * Start running the Threadable object
   */
  void start() const;

  /**
   * Cancel execution of the Threadable object
   *
   * Because cancellation is not abortion, it is up to the
   * Threadable to accept cancellation. So calling this method
   * does not mean that the Thread will actually stop
   */
  void cancel() const;

  /**
   * Get a ThreadStatus object that informs the user of
   * the internal status of the current thread
   */
  const ThreadStatus getStatus() const;

protected:

private:

  /** The Threadable object this thread must run */
  const Threadable & threadable_;

  /** Copy constructor */
  Thread(const Thread & other);

  /** Assignment */
  Thread & operator =(const Thread & other);

}

  ; /* class Thread */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_THREAD_HXX */
