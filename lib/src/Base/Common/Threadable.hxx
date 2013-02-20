//                                               -*- C++ -*-
/**
 *  @file  Threadable.hxx
 *  @brief The class Threadable defines an interface for multithreaded objects
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
 *  Id      Threadable.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_THREADABLE_HXX
#define OPENTURNS_THREADABLE_HXX

#include "OTprivate.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class Threadable
 * @brief Defined an interface for multithreaded classes
 * @internal
 *
 * Threadable is an interface (an abstract class) intended to
 * be derived by any class that need to be run in a multithreaded
 * environment.
 */

class Threadable
{
public:

  /** Destructor */
  virtual ~Threadable();

  /**
   * The run method must be overloaded and populated with what
   * the actual (derived) class must do
   */
  virtual void run() = 0;
}

  ; /* class Threadable */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_THREADABLE_HXX */
