//                                               -*- C++ -*-
/**
 *  @file  OStream.hxx
 *  @brief The class Ostream is an helper for pretty printing of Objects
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
 *  @date   2009-04-07 12:06:29 +0200 (Tue, 07 Apr 2009)
 *  Id      OStream.hxx 1205 2009-04-07 10:06:29Z dutka
 */
#ifndef OPENTURNS_OSTREAM_HXX
#define OPENTURNS_OSTREAM_HXX

#include <iostream>              // for std::ostream
#include <iomanip>               // for manipulators like setw, setprecision, etc.
#include "OTprivate.hxx"
#include "Object.hxx"

BEGIN_NAMESPACE_OPENTURNS



// This class defines an new ostream based stream
class OStream
{
  std::ostream & os_;
public:
  explicit OStream(std::ostream & os) : os_(os) {}
  std::ostream & getStream()
  {
    return os_;
  }
}; // end class OStream

// This operator writes Object derived objects to OStream
inline
OStream & operator << (OStream & OS, const Object & obj)
{
  OS.getStream() << obj.__repr__();
  return OS;
}

inline
OStream & operator << (OStream & OS, const String & st)
{
  OS.getStream() << st;
  return OS;
}

inline
OStream & operator << (OStream & OS, const char * ch)
{
  OS.getStream() << ch;
  return OS;
}

inline
OStream & operator << (OStream & OS, int val)
{
  OS.getStream() << val;
  return OS;
}

inline
OStream & operator << (OStream & OS, long val)
{
  OS.getStream() << val;
  return OS;
}

inline
OStream & operator << (OStream & OS, long long val)
{
  OS.getStream() << val;
  return OS;
}

inline
OStream & operator << (OStream & OS, short val)
{
  OS.getStream() << val;
  return OS;
}

inline
OStream & operator << (OStream & OS, unsigned int val)
{
  OS.getStream() << val;
  return OS;
}

inline
OStream & operator << (OStream & OS, unsigned long val)
{
  OS.getStream() << val;
  return OS;
}

inline
OStream & operator << (OStream & OS, unsigned long long val)
{
  OS.getStream() << val;
  return OS;
}

inline
OStream & operator << (OStream & OS, unsigned short val)
{
  OS.getStream() << val;
  return OS;
}

inline
OStream & operator << (OStream & OS, char val)
{
  OS.getStream() << val;
  return OS;
}

inline
OStream & operator << (OStream & OS, bool val)
{
  OS.getStream() << (val ? "true" : "false");
  return OS;
}

inline
OStream & operator << (OStream & OS, float val)
{
  OS.getStream() << val;
  return OS;
}

inline
OStream & operator << (OStream & OS, double val)
{
  OS.getStream() << val;
  return OS;
}

inline
OStream & operator << (OStream & OS, long double val)
{
  OS.getStream() << val;
  return OS;
}

inline
OStream & operator << (OStream & OS, const NumericalComplex & c)
{
  OS.getStream() << c;
  return OS;
}

inline
OStream & operator << (OStream & OS, const void * ptr)
{
  OS.getStream() << ptr;
  return OS;
}

inline
OStream & operator << (OStream & OS, std::ostream & (*manip)(std::ostream &))
{
  OS.getStream() << manip;
  return OS;
}

inline
OStream & operator << (OStream & OS, std::ios_base & (*manip)(std::ios_base &))
{
  OS.getStream() << manip;
  return OS;
}

inline
OStream & operator << (OStream & OS, std::_Setw manip)
{
  OS.getStream() << manip;
  return OS;
}

inline
OStream & operator << (OStream & OS, std::_Setprecision manip)
{
  OS.getStream() << manip;
  return OS;
}

inline
OStream & operator << (OStream & OS, std::_Setbase manip)
{
  OS.getStream() << manip;
  return OS;
}


inline
OStream & operator << (OStream & OS, std::_Resetiosflags manip)
{
  OS.getStream() << manip;
  return OS;
}

inline
OStream & operator << (OStream & OS, std::_Setiosflags manip)
{
  OS.getStream() << manip;
  return OS;
}

inline
OStream & operator << (OStream & OS, std::_Setfill<char> manip)
{
  OS.getStream() << manip;
  return OS;
}


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_OSTREAM_HXX */
