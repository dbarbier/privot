//                                               -*- C++ -*-
/**
 *  @file  ThreadStatus.cxx
 *  @brief The class ThreadStatus is the top-most class of Open TURNS
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
#include "ThreadStatus.hxx"
#include "OSS.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(ThreadStatus);

/* Default constructor */
ThreadStatus::ThreadStatus()
  : Object()
{
  // Nothing to do
}

/* String converter */
String ThreadStatus::__repr__() const
{
  return OSS() << "class=" << getClassName();
}




END_NAMESPACE_OPENTURNS
