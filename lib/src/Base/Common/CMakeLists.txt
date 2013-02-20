#                                               -*- cmake -*-
#
#  CMakeLists.txt
#
#  Copyright (C) 2005-2013 EDF-EADS-Phimeca
#
#  This library is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  along with this library.  If not, see <http://www.gnu.org/licenses/>.
#
#  @author dutka
#  @date   2010-02-04 16:44:49 +0100 (Thu, 04 Feb 2010)
#

# Register current directory files
ot_add_current_dir_to_include_dirs ()

ot_add_source_file ( Memory.cxx )
ot_add_source_file ( OSS.cxx )
ot_add_source_file ( PlatformInfo.cxx )
ot_add_source_file ( AtomicFunctions.cxx )
ot_add_source_file ( Object.cxx )
ot_add_source_file ( PersistentObject.cxx )
ot_add_source_file ( IdFactory.cxx )
ot_add_source_file ( Catalog.cxx )
ot_add_source_file ( Exception.cxx )
ot_add_source_file ( InterfaceObject.cxx )
ot_add_source_file ( Lockable.cxx )
ot_add_source_file ( Threadable.cxx )
ot_add_source_file ( ThreadStatus.cxx )
ot_add_source_file ( Thread.cxx )
ot_add_source_file ( StorageManager.cxx )
ot_add_source_file ( Os.cxx )
ot_add_source_file ( Path.cxx )
ot_add_source_file ( ComparisonOperator.cxx )
ot_add_source_file ( ComparisonOperatorImplementation.cxx )
ot_add_source_file ( Less.cxx )
ot_add_source_file ( LessOrEqual.cxx )
ot_add_source_file ( Equal.cxx )
ot_add_source_file ( Greater.cxx )
ot_add_source_file ( GreaterOrEqual.cxx )
ot_add_source_file ( ResourceMap.cxx )
ot_add_source_file ( Log.cxx )
ot_add_source_file ( MPI.cxx )
ot_add_source_file ( TTY.cxx )
ot_add_source_file ( TBB.cxx )
ot_add_source_file ( Study.cxx )
ot_add_source_file ( WrapperCommon.cxx )
ot_add_source_file ( WrapperCommonFunctions.cxx )

if (LIBXML2_FOUND)
  ot_add_source_file ( XMLStorageManager.cxx )
  ot_add_source_file ( XMLToolbox.cxx )
endif (LIBXML2_FOUND)

#ot_install_header_file ( WrapperCommonFunctions.hxx )
ot_install_header_file ( Greater.hxx )
ot_install_header_file ( ResourceMap.hxx )
ot_install_header_file ( Thread.hxx )
ot_install_header_file ( ComparisonOperatorImplementation.hxx )
ot_install_header_file ( OStream.hxx )
ot_install_header_file ( SharedPointer.hxx )
ot_install_header_file ( StorageManager.hxx )
ot_install_header_file ( OSS.hxx )
ot_install_header_file ( Study.hxx )
ot_install_header_file ( Less.hxx )
ot_install_header_file ( PersistentObject.hxx )
ot_install_header_file ( PlatformInfo.hxx )
ot_install_header_file ( AtomicFunctions.hxx )
ot_install_header_file ( MutexLock.hxx )
ot_install_header_file ( Threadable.hxx )
ot_install_header_file ( Equal.hxx )
ot_install_header_file ( Pointer.hxx )
ot_install_header_file ( ThreadStatus.hxx )
ot_install_header_file ( IdFactory.hxx )
ot_install_header_file ( Memory.hxx )
ot_install_header_file ( PersistentObjectFactory.hxx )
ot_install_header_file ( Lockable.hxx )
ot_install_header_file ( ComparisonOperator.hxx )
ot_install_header_file ( TypedInterfaceObject.hxx )
ot_install_header_file ( InterfaceObject.hxx )
ot_install_header_file ( OTCommon.hxx )
ot_install_header_file ( Catalog.hxx )
ot_install_header_file ( GreaterOrEqual.hxx )
ot_install_header_file ( Log.hxx )
ot_install_header_file ( MPI.hxx )
ot_install_header_file ( TBB.hxx )
ot_install_header_file ( TypedCollectionInterfaceObject.hxx )
ot_install_header_file ( Os.hxx )
ot_install_header_file ( Object.hxx )
ot_install_header_file ( LessOrEqual.hxx )
ot_install_header_file ( Exception.hxx )
ot_install_header_file ( Path.hxx )
ot_install_header_file ( TTY.hxx )
ot_install_header_file ( Wrapper.h )
ot_install_header_file ( WrapperInterface.h )
ot_install_header_file ( WrapperInterface_V0.h )
ot_install_header_file ( WrapperCommon.h )
ot_install_header_file ( WrapperCommon_macros.h )
ot_install_header_file ( WrapperCommon_static.h )
ot_install_header_file ( WrapperMacros.h )

if (LIBXML2_FOUND)
  ot_install_header_file ( XMLStorageManager.hxx )
  ot_install_header_file ( XMLToolbox.hxx )
  ot_install_header_file ( XMLchar_traits.hxx )
endif (LIBXML2_FOUND)

