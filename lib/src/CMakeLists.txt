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
ot_install_header_file ( OT.hxx )
ot_add_source_file ( openturns_library_ok.c )

foreach ( include_dir ${OPENTURNS_INCLUDE_DIRS} )
  set ( EXTRA_CPPFLAGS "${EXTRA_CPPFLAGS} -I${include_dir}" )
endforeach ()
configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/openturns-config-build.in
                 ${CMAKE_CURRENT_BINARY_DIR}/openturns-config-build
                 @ONLY
               )
set (WGET wget)
set (CURL curl)
set (prefix "${CMAKE_INSTALL_PREFIX}")
set (exec_prefix "${CMAKE_INSTALL_PREFIX}")
set (libdir "${INSTALL_PATH}/lib${LIB_SUFFIX}")
set (PACKAGE "${CPACK_PACKAGE_NAME}")
configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/openturns-module-build.in
                 ${CMAKE_CURRENT_BINARY_DIR}/openturns-module-build
                 @ONLY
               )
install ( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/openturns-config-build
          DESTINATION ${INSTALL_PATH}/bin
          RENAME openturns-config
        )
install ( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/openturns-module-build
          DESTINATION ${INSTALL_PATH}/bin
          RENAME openturns-module
        )

# Recurse in subdirectories
add_subdirectory ( Base )
add_subdirectory ( Uncertainty )
if ( NOT LAPACK_FOUND AND NOT BLAS_FOUND )
  add_subdirectory ( BlasLapack )
endif ( NOT LAPACK_FOUND AND NOT BLAS_FOUND )

# Build main library
foreach ( built_source ${BUILT_SOURCEFILES} )
  set_source_files_properties ( ${built_source} PROPERTIES GENERATED TRUE )
endforeach ( built_source )
include_directories ( ${INTERNAL_INCLUDE_DIRS} )
link_directories ( ${CMAKE_Fortran_IMPLICIT_LINK_DIRECTORIES} )
add_library ( OTbind SHARED bind.c )
if ( MINGW )
  set_target_properties ( OTbind PROPERTIES
                                  LINK_FLAGS "-Wl,--out-implib,libOTbind.dll.a" )
endif ( MINGW )
add_library ( OT SHARED ${SOURCEFILES} ${BUILT_SOURCEFILES} )
set_target_properties ( OT OTbind PROPERTIES SOVERSION 0 )
set_target_properties ( OT OTbind PROPERTIES VERSION 0.0.0 )
if ( BISON_FOUND )
  add_dependencies ( OT generate_csv_parser )
endif ( BISON_FOUND )
if ( FLEX_FOUND )
  add_dependencies ( OT generate_csv_lexer  )
endif ( FLEX_FOUND )

# CMake makefiles don't link against stdc++ on OSX
if ( APPLE )
  target_link_libraries ( OTbind stdc++ )
endif ( APPLE )

target_link_libraries ( OT ${OPENTURNS_LIBRARIES} )

# Build an empty executable to check link dependencies and completeness
add_executable ( linktest main.cxx )
target_link_libraries ( linktest OT )
set_target_properties ( linktest PROPERTIES
                                 INSTALL_RPATH "${CMAKE_INSTALL_RPATH};${CMAKE_BINARY_DIR}/lib/src" )


# Install rules for library and executable
install ( TARGETS OT OTbind
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION ${LIBRARY_PATH}
          ARCHIVE DESTINATION ${LIBRARY_PATH}
        )

#foreach ( file ${SOURCEFILES} )
#  message ( ">> Will compile ${file}" )
#endforeach ( file )

#foreach ( file ${BUILT_SOURCEFILES} )
#  message ( ">> Will compile built ${file}" )
#endforeach ( file )

#foreach ( file ${HEADERFILES} )
#  message ( ">> Will install ${file}" )
#endforeach ( file )

#get_cmake_property ( VARS VARIABLES )
#foreach ( var ${VARS} )
#  file( APPEND ${CMAKE_CURRENT_BINARY_DIR}/VARS.txt
#        "${var} \"${${var}}\"\n" )
#endforeach ( var )
