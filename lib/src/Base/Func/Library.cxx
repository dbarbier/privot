//                                               -*- C++ -*-
/**
 *  @file  Library.cxx
 *  @brief A Library object points to an actual shared library
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
#include "OTconfig.hxx"
#include "OTdebug.h"
#ifdef HAVE_LIBDL
BEGIN_C_DECLS
#include <dlfcn.h>               // for dlopen, dlsym, dlclose, dlerror
END_C_DECLS
#endif
#include "Library.hxx"
#include "Log.hxx"

BEGIN_NAMESPACE_OPENTURNS




CLASSNAMEINIT(Library);

/* Default constructor */
Library::Library(LibraryHandle handle,
                 const String & path)
  : PersistentObject(),
    handle_(handle),
    path_(path)
{
  // Nothing to do
}

/* Virtual constructor */
Library * Library::clone() const
{
  return new Library(*this);
}

/* Comparison operator */
Bool Library::operator ==(const Library & other) const
{
  return true;
}

/* String converter */
String Library::__repr__() const
{
  return OSS() << "class=" << getClassName()
               << " name=" << getName()
               << " path=" << path_
               << " handle=" << handle_;
}

/* String converter */
String Library::__str__(const String & offset) const
{
  return OSS(false) << offset << path_;
}


/* Library symbol accessor */
LibrarySymbol Library::getSymbol(String symbolName) const
{
#ifdef HAVE_LIBDL
  // We first clear any previous error
  dlerror();

  char * error;

  // Now we look for the symbol in the library
  LibrarySymbol symbol = dlsym(handle_, symbolName.c_str());
  if ((error = dlerror()) != NULL)
    throw DynamicLibraryException(HERE) << "Cannot dlsym symbol '"
                                        << symbolName << "' in library located at '"
                                        << path_ << "'. Reason : "
                                        << error;
  LOGDEBUG(OSS() << "Found symbol '" << symbolName << "' (" << symbol << ") in library located at '" << path_ << "'");

  return symbol;
#else /* HAVE_LIBDL */
  throw NotYetImplementedException(HERE) << "Library::getSymbol() need libdl";
#endif /* HAVE_LIBDL */
}





END_NAMESPACE_OPENTURNS
