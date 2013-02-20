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

include ( FortranCInterface )
FortranCInterface_HEADER ( FortranCInterface.h )

ot_add_current_dir_to_include_dirs ()
ot_add_build_dir_to_include_dirs ( ${CMAKE_CURRENT_BINARY_DIR} )

configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/OTconfig.h.in
                 ${CMAKE_CURRENT_BINARY_DIR}/OTconfig.h
                @ONLY
               )

set ( ot_configure_args )
if ( NOT ot_configure_date )
  execute_process ( COMMAND date -R
                    OUTPUT_VARIABLE ot_configure_date
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                  )
  set ( ot_configure_date "${ot_configure_date}" CACHE STRING "First configure date." )
endif ()
execute_process ( COMMAND svnversion
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                  OUTPUT_VARIABLE ot_revision
                  OUTPUT_STRIP_TRAILING_WHITESPACE
                )
set ( prefix ${CMAKE_INSTALL_PREFIX} )
set ( PACKAGE openturns )

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/OTconfigure_args.hxx.in
                ${CMAKE_CURRENT_BINARY_DIR}/OTconfigureArgs.hxx
                @ONLY
              )

install ( FILES OTconfig.hxx
                OTdebug.h
                OTmemory.hxx
                OTprivate.hxx
                OTtestcode.hxx
                OTthread.hxx
                OTtypes.hxx
                ${CMAKE_CURRENT_BINARY_DIR}/OTconfig.h
          DESTINATION ${OPENTURNS_INCLUDE_PATH}
          PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
        )


#get_cmake_property ( VARS VARIABLES )
#foreach ( var ${VARS} )
#  file( APPEND ${CMAKE_CURRENT_BINARY_DIR}/VARS.txt
#        "${var} \"${${var}}\"\n" )
#endforeach ( var )
