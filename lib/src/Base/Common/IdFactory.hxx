//                                               -*- C++ -*-
/**
 *  @file  IdFactory.hxx
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
 *  Id      IdFactory.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_IDFACTORY_HXX
#define OPENTURNS_IDFACTORY_HXX

#include <iostream>              // for std::ostream
#include "OTprivate.hxx"
#include "AtomicFunctions.hxx"
#include "OStream.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class IdFactory
 *
 * @brief %IdFactory builds unique Ids for all PersistentObjects
 * @see PersistentObject
 * @see StorageManager
 *
 * The IdFactory ensures that each PersistentObject has an unique Id so they can be
 * distinguished from each other.
 * The IdFactory implements a 'singleton' strategy.
 */

class IdFactory
{
public:

  /** Id accessor
   *
   * The way to get an Id for a new PersistentObject.
   */
  static Id BuildId();


private:

  /** Default constructor */
  IdFactory();

  friend struct IdFactory_init;
}; /* class IdFactory */

/** This struct initializes all static members of IdFactory */
struct IdFactory_init
{
  IdFactory_init();
}; /* end struct IdFactory_init */

static IdFactory_init __IdFactory_initializer;

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_IDFACTORY_HXX */
