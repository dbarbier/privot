//                                               -*- C++ -*-
/**
 *  @file  ResourceMap.hxx
 *  @brief ResourceMap defines a resource catalog
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
 *  Id      ResourceMap.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_RESOURCEMAP_HXX
#define OPENTURNS_RESOURCEMAP_HXX

#include <map>
#include "OStream.hxx"
#include "Pointer.hxx"
#include "Path.hxx"
#include "MutexLock.hxx"

BEGIN_NAMESPACE_OPENTURNS



/** Forward declaration */
class ResourceMap;

class ResourceMapInstance
{
  ResourceMap & rm_;
  MutexLock lock_;

public:
  ResourceMapInstance( ResourceMap & rm ) throw();
  ResourceMapInstance( const ResourceMapInstance & other );

  /** @copydoc Object::__repr__() const */
  String __repr__() const;

#ifndef SWIG
  ResourceMap & lock() throw();
  const ResourceMap & lock() const throw();
#endif
}; // end class ResourceMapInstance

/**
 * @class ResourceMap
 * @brief Defines a catalog containing all default values used by Open TURNS
 *
 * As Open TURNS uses many default values for its computations, their actual values are
 * accessible and editable through this class. Some default values are hardcoded in this class,
 * some other are read in an configuration file.
 */

class ResourceMap
{
public:

  /** Since ResourceMap is a singleton, GetInstance gives access to the object */
  static ResourceMapInstance GetInstance();
  static void Release();
  static void Initialize();

  /** Get a value in the map */
  static String Get(String key);
  static Bool GetAsBool(String key);
  static UnsignedLong GetAsUnsignedLong(String key);
  static NumericalScalar GetAsNumericalScalar(String key);
  static char * GetAsNewCharArray(String key);

  /** Set a value in the map */
  static void Set(String key, String value);
  static void SetAsBool(String key, Bool value);
  static void SetAsUnsignedLong(String key, UnsignedLong value);
  static void SetAsNumericalScalar(String key, NumericalScalar value);

  /** @copydoc Object::__repr__() const */
  String __repr__() const;

protected:

  /** Method for retrieving information from the resource map
   * @param key The name under which the value is stored in the ResourceMap
   * @return The value written into a string
   */
  String get(String key) const;

  /** Method for retrieving information from the resource map
   * @param key The name under which the value is stored in the ResourceMap
   * @return The value if the value is boolean castable, false otherwise
   */
  Bool getAsBool(String key) const;

  /** Method for retrieving information from the resource map
   * @param key The name under which the value is stored in the ResourceMap
   * @return The value if the value is integer castable, zero otherwise
   */
  UnsignedLong getAsUnsignedLong(String key) const;

  /** Method for retrieving information from the resource map
   * @param key The name under which the value is stored in the ResourceMap
   * @return The value if the value is double castable, zero otherwise
   */
  NumericalScalar getAsNumericalScalar(String key) const;

  /** Method for retrieving information from the resource map
   * @param key The name under which the value is stored in the ResourceMap
   * @return The value written into a new allocated const char * (use free() to deallocate)
   */
  char * getAsNewCharArray(String key) const;

  /** Method for setting information into the resource map
   * @param key The name under which the value is stored in the ResourceMap
   * @param value The value written to a string
   */
  void set(String key, String value);

  /** Method for setting information into the resource map
   * @param key The name under which the value is stored in the ResourceMap
   * @param value The value as a boolean
   */
  void setAsBool(String key, Bool value);

  /** Method for setting information into the resource map
   * @param key The name under which the value is stored in the ResourceMap
   * @param value The value as an integer
   */
  void setAsUnsignedLong(String key, UnsignedLong value);

  /** Method for setting information into the resource map
   * @param key The name under which the value is stored in the ResourceMap
   * @param value The value as a double
   */
  void setAsNumericalScalar(String key, NumericalScalar value);

  /** Update the ResourceMap with information from the configuration file */
  void readConfigurationFile(const FileName & configurationFile);

  /** Find the configuration file in specific path (see Path class for algorithm) */
  FileName findConfigurationFile() const;

  /** Load the configuration file */
  void loadConfigurationFile();

  /** Load the configuration defined at installation time */
  void loadDefaultConfiguration();

private:

  /** Default constructor */
  ResourceMap();

  /** Default constructor */
  ResourceMap(const ResourceMap & other) : map_(other.map_) {}

  /** The actual map that stores the key/value pairs */
  typedef std::map< String, String > MapType;
  MapType map_;

  friend struct ResourceMap_init;
}; /* class ResourceMap */

/** This struct initializes all static members of ResourceMap */
struct ResourceMap_init
{
  ResourceMap_init();
  ~ResourceMap_init();
};
static ResourceMap_init __ResourceMap_initializer;

/**
 * @fn std::ostream & operator <<(std::ostream & os, const ResourceMap & obj)
 * @brief Output stream converter
 * @param os A STL output stream resourceMap
 * @param obj The resourceMap read by \em os
 * @return A reference to \em os
 *
 * Operator << converts the ResourceMap object to an output stream
 * so it is easy to show the content of the resourceMap.
 */
std::ostream & operator <<(std::ostream & os, const ResourceMapInstance & obj);
#ifndef SWIG
OStream & operator <<(OStream & OS, const ResourceMapInstance & obj);
#endif


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_RESOURCEMAP_HXX */
