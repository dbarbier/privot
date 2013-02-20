//                                               -*- C++ -*-
/**
 *  @file  Object.hxx
 *  @brief The class Object is the top-most class of Open TURNS
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
 *  Id      Object.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_OBJECT_HXX
#define OPENTURNS_OBJECT_HXX

#include <iostream>              // for std::ostream
#include <sstream>               // for std::ostringstream
#include "OTprivate.hxx"



/** Declare GetClassName() and getClassName() for Object-like classes */

#define CLASSNAME                               \
  public:                                       \
  /** Class name accessor */                    \
  static  OT::String GetClassName();            \
  virtual OT::String getClassName() const;      \

/** Define GetClassName() and getClassName() for Object-like classes */

#define CLASSNAMEINIT(T)                                                \
  OT::String T::GetClassName()       { return OT::String(#T); }         \
  OT::String T::getClassName() const { return T::GetClassName(); }      \


#define TEMPLATE_CLASSNAMEINIT(T)                                       \
  template<> OT::String T::GetClassName()       { return OT::String(#T); } \
  template<> OT::String T::getClassName() const { return T::GetClassName(); } \



BEGIN_NAMESPACE_OPENTURNS



/**
 * @class Object
 *
 * @brief The base classe for almost every object in Open TURNS.
 *
 * Object defines common methods needed by all classes.
 * These are default constructor, copy constructor, virtual
 * destructor, affectation operator, comparison operator and
 * string converters.
 * These methods should be overloaded by every derived class
 */

class Object
{
  CLASSNAME;
public:

  /** Destructor */
  virtual ~Object() = 0;

  /**
   * String converter
   *
   * This method shows human readable information on the
   * internal state of an object. It is used when streaming
   * the object or for user information.
   *
   * @return A string fully describing the object
   */
  virtual String __repr__() const;


  /**
   * Pretty string converter
   *
   * This method shows human readable information on the
   * internal state of an object. It is used for printing the object
   * and should only show relevant information.
   *
   * @return A human readable string
   */
  virtual String __str__(const String & offset = "") const;


protected:

private:

} ; /* class Object */

#ifndef SWIG
/**
 * @fn std::ostream & operator <<(std::ostream & os, const Object & obj)
 * @brief Output stream converter
 * @param os A STL output stream object
 * @param obj The object read by \p os
 * @return A reference to \p os
 *
 * This method allows any Object-derived class to be streamed whitout
 * redefining this method. Each derived-class should have a public
 * method str() that returns a String.
 *
 * @sa String __repr__() const
 */
std::ostream & operator <<(std::ostream & os, const Object & obj);

#endif

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_OBJECT_HXX */
