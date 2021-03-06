//                                               -*- C++ -*-
/**
 *  @file  Catalog.hxx
 *  @brief Catalog records all the factories into a dictionnary
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
 *  Id      Catalog.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_CATALOG_HXX
#define OPENTURNS_CATALOG_HXX

#include <map>         // for std::map
#include "OTprivate.hxx"


BEGIN_NAMESPACE_OPENTURNS



/** Forward declaration */
class PersistentObjectFactory;

/**
 * @class Catalog
 *
 * @brief %Catalog records all the object's factories into a dictionnary.
 *
 * %Catalog implements the 'singleton' strategy to share an unique dictionnary.
 * The catalog is readonly for common usage but writable only for factories
 * that record themselves within their constructor.
 */

class Catalog
{
private:

  /** These methods allocate and free storage */
  static void Release();
  static void Initialize();


public:

  /** Return the catalog as a singleton */
  static Catalog & GetInstance();

  /** Return the catalog as a singleton */
  static void Add(const String & factoryName, const PersistentObjectFactory * p_factory);

  /** Get the factory by name */
  static const PersistentObjectFactory & Get(const String & factoryName);


  /** Destructor */
  ~Catalog();

  /** @copydoc Object::__repr__() const */
  String __repr__() const;

private:

  typedef std::map<String, const PersistentObjectFactory *> CatalogType;

  /** The dictionnary of factories */
  CatalogType catalog_;

  /** Default constructor is private so Catalog can not be instanciated */
  Catalog();

  /** Add a new factory to the catalog */
  void add(const String & factoryName, const PersistentObjectFactory * p_factory);

  /** Get the factory by name */
  const PersistentObjectFactory & get(const String & factoryName) const;

  friend struct Catalog_init; /* friendship for static member initialization */
}; /* end class Catalog */

/** This class initializes all static members of Catalog */
struct Catalog_init
{
  Catalog_init();
  ~Catalog_init();
}; /* end struct Catalog_init */

static Catalog_init __Catalog_initializer;



END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_CATALOG_HXX */
