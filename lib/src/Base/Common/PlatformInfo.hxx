//                                               -*- C++ -*-
/**
 *  @file  PlatformInfo.hxx
 *  @brief The class PlatformInfo gives information about the library
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
 *  Id      PlatformInfo.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_PLATFORMINFO_HXX
#define OPENTURNS_PLATFORMINFO_HXX

#include "OTprivate.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class PlatformInfo
 * @brief Defines static methods to query the library about various information (name, version, etc.)
 */

class PlatformInfo
{
private:
  /**
   * The numerical precision used to write out scientific numbers
   */
  static int Precision_;

public:

  /** Return the version number of Open TURNS */
  static String GetVersion();
  /** Return the name of the package Open TURNS was built from */
  static String GetName();
  /** Return the revision of the versioning system this version corresponds to (broken for the moment) */
  static String GetRevision();
  /** Return the date Open TURNS was built */
  static String GetDate();
  /** Return the path Open TURNS was installed to */
  static String GetInstallationDirectory();
  /** Return the path of the module standard directory */
  static String GetModuleDirectory();
  /** Return the command line used to configure Open TURNS */
  static String GetConfigureCommandLine();
  /** Return the precision used for floating %point output */
  static int    GetNumericalPrecision();
  /** Define the precision used for floating %point output
   * @param precision An positive integer counting the number of digits shown
   */
  static void   SetNumericalPrecision(int precision);

private:

  int precision_;

public:

  PlatformInfo();  // Required by SWIG
  ~PlatformInfo(); // Required by SWIG

  /** @copydoc GetVersion() */
  String getVersion() const;
  /** @copydoc GetName() */
  String getName() const;
  /** @copydoc GetRevision() */
  String getRevision() const;
  /** @copydoc GetDate() */
  String getDate() const;
  /** @copydoc GetInstallationDirectory() */
  String getInstallationDirectory() const;
  /** @copydoc GetModuleDirectory() */
  String getModuleDirectory() const;
  /** @copydoc GetConfigureCommandLine() */
  String getConfigureCommandLine() const;
  /** @copydoc GetNumericalPrecision() */
  int    getNumericalPrecision() const;
  /** @copydoc SetNumericalPrecision() */
  void   setNumericalPrecision(int precision);

  /** @copydoc Object::__repr__() const */
  String __repr__() const;

  /** @copydoc Object::__str__() const */
  String __str__() const;

}; /* class PlatformInfo */



END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_PLATFORMINFO_HXX */
