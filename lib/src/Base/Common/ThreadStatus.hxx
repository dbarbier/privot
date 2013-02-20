//                                               -*- C++ -*-
/**
 *  @file  ThreadStatus.hxx
 *  @brief The class ThreadStatus owns Thread object internal information
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
 *  Id      ThreadStatus.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_THREADSTATUS_HXX
#define OPENTURNS_THREADSTATUS_HXX

#include <iostream>              // for std::ostream
#include "OTprivate.hxx"
#include "Object.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class ThreadStatus
 * @brief Defined internal thread information
 * @internal
 *
 * ThreadStatus objects own Thread object internal information like
 * thread identifier, internal state, related Threadable object, etc.
 * Those information are accessible readonly by the user.
 */

class ThreadStatus
  : public Object
{
  CLASSNAME;
public:

  /**
   * Default constructor
   *
   * The default constructor allows the object to be
   * stored in STL containers like vector or map.
   */
  explicit ThreadStatus();

  /** @copydoc Object::__repr__() const */
  virtual String __repr__() const;

}; /* class Threadstatus */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_THREADSTATUS_HXX */
