//                                               -*- C++ -*-
/**
 *  @file  WrapperFile_version1.hxx
 *  @brief This class provides the elementary functions for wrapper file management (version 1)
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
 *  @author dutka
 *  @date   2011-02-08 23:15:59 +0100 (Tue, 08 Feb 2011)
 *  Id      WrapperFile.cxx 1750 2011-02-08 22:15:59Z dutka
 */
#include "OTprivate.hxx"
#include "WrapperData.hxx"
#include "XMLToolbox.hxx"

#ifdef XML_SUPPORTED

BEGIN_NAMESPACE_OPENTURNS

/* Forward declaration */
class WrapperFile;

/** Get wrapper DTD file path */
FileName GetDTDFileName_version1();

/** Document parsing function */
WrapperData ParseDocument_version1(const XMLDoc & doc);

/** Make a new document from the internal data */
XMLDoc MakeDocument_version1(const WrapperFile & wrapperFile);

END_NAMESPACE_OPENTURNS

#endif /* XML_SUPPORTED */
