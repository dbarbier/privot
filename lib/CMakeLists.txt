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

#project ( OpenTURNSLib CXX C Fortran )

# Recurse in subdirectories
add_subdirectory ( include )
add_subdirectory ( src )
add_subdirectory ( etc )

# Build the OpenTURNSConfig.cmake file
get_target_property ( OPENTURNS_LIBRARY_LOC OT LOCATION )
get_filename_component ( OPENTURNS_INSTALL_LIBRARY ${OPENTURNS_LIBRARY_LOC} NAME )

if ( MINGW )
  set ( OPENTURNS_INSTALL_LIBRARY ${OPENTURNS_INSTALL_LIBRARY}.a )
endif ( MINGW )

set ( OPENTURNS_LIBRARY_DIRS            ${LIBRARY_PATH} )
list ( INSERT OPENTURNS_LIBRARIES 0     ${LIBRARY_PATH}/${OPENTURNS_INSTALL_LIBRARY} )
list ( INSERT OPENTURNS_INCLUDE_DIRS 0  ${INCLUDE_PATH} )

set ( OPENTURNS_ROOT_DIR       ${INSTALL_PATH} )
set ( OPENTURNS_VERSION_STRING ${PACKAGE_VERSION} )
set ( OPENTURNS_VERSION_MAJOR  ${CPACK_PACKAGE_VERSION_MAJOR} )
set ( OPENTURNS_VERSION_MINOR  ${CPACK_PACKAGE_VERSION_MINOR} )
set ( OPENTURNS_VERSION_PATCH  ${CPACK_PACKAGE_VERSION_PATCH} )
set ( OPENTURNS_USE_FILE       ${CONFIG_CMAKE_PATH}/UseOpenTURNS.cmake )

if ( LIBXML2_FOUND )
  list ( APPEND OPENTURNS_DEFINITIONS -DHAVE_LIBXML2 )
endif ( LIBXML2_FOUND)
if ( Threads_FOUND OR MINGW )
  list ( APPEND OPENTURNS_DEFINITIONS -DHAVE_PTHREAD_H )
endif ()
list ( APPEND OPENTURNS_DEFINITIONS -DPACKAGE_VERSION=\"${PACKAGE_VERSION}\" )
list ( APPEND OPENTURNS_DEFINITIONS -DPACKAGE_NAME=\"${PACKAGE_NAME}\" )
configure_file ( ${PROJECT_SOURCE_DIR}/lib/OpenTURNSConfig.cmake.in
                 ${PROJECT_BINARY_DIR}/lib/OpenTURNSConfig.cmake
                @ONLY ESCAPE_QUOTES
               )

install ( FILES ${PROJECT_SOURCE_DIR}/lib/UseOpenTURNS.cmake
                ${PROJECT_BINARY_DIR}/lib/OpenTURNSConfig.cmake
          DESTINATION ${OPENTURNS_CONFIG_CMAKE_PATH}
        )

install ( FILES AUTHORS
                COPYING
                COPYING.MuParser
                COPYING.cobyla
                COPYING.dsfmt
                COPYING.lapack
                COPYING.tnc
                COPYING.kissfft
                ChangeLog
                NEWS
                README
                TODO
          DESTINATION ${OPENTURNS_DOC_PATH}
        )

# Recurse in subdirectories : test should be run after src (for target OT to be built)
add_subdirectory ( test )

if ( DOXYGEN_FOUND )

  # generate Doxyfile
  set ( VERSION ${PACKAGE_VERSION} )
  set ( builddir ${CMAKE_CURRENT_BINARY_DIR} )
  set (  top_srcdir ${CMAKE_CURRENT_SOURCE_DIR} )
  configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
                   ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                    @ONLY )
                    
  # generate html doc and compress it
  add_custom_target ( html 
                      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/doc
                      COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
                      COMMAND tar czf doc-api-${PACKAGE_VERSION}.tar.gz doc
                      COMMAND tar cjf doc-api-${PACKAGE_VERSION}.tar.bz2 doc
                      )                   
                      
endif ( DOXYGEN_FOUND )
