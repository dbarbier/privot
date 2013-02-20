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

cmake_minimum_required ( VERSION 2.8 )

option ( USE_BISON                    "Looks for Bison if true and then build parser"                         ON )
option ( USE_FLEX                     "Looks for Flex if true and then build lexer"                           ON )
option ( USE_TBB                      "Use Intel Threading Building Blocks library for multithreading"        ON )
option ( USE_INTERNAL_BLASLAPACK      "Force usage of internal BLAS/LAPACK libraries"                         OFF )
option ( USE_INTERNAL_MUPARSER        "Force usage of internal muParser libraries"                            ON )
option ( USE_LIBXML2                  "Use LibXml2 for XML support"                                           ON )
option ( USE_R                        "Use R for graph output support"                                        ON )
option ( ENABLE_DISTRIBUTED_TEST      "Check distributed python wrapper on remote host if true."              OFF )
option ( OPENTURNS_SYSTEM_INSTALL     "True if Open TURNS is installed in a system directory. RPATH removed." OFF )
option ( OPENTURNS_BUILD_LIB          "Build the library"                                                     ON )
option ( OPENTURNS_BUILD_PYTHON       "Build the python module for the library"                               ON )
option ( OPENTURNS_BUILD_VALIDATION   "Build the validation files of the library"                             ON )

# By default, build in Release mode.  Must appear before project() command
if (DEFINED CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
else (DEFINED CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
endif (DEFINED CMAKE_BUILD_TYPE)

if ( NOT DEFINED SITE_SUBDIR )
  set ( SITE_SUBDIR /site-packages )
  # debian install system packages into .../dist-packages
  if ( EXISTS "/etc/debian_version" )
    if ( OPENTURNS_SYSTEM_INSTALL OR NOT DEFINED CMAKE_INSTALL_PREFIX )
      set ( SITE_SUBDIR /dist-packages )
    endif ()
  endif ()
endif ()

project ( OpenTURNS CXX C Fortran )


# Defines our own module path
list ( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )

if ( USE_BISON )
  find_package ( BISON REQUIRED )
endif ()

if ( USE_FLEX )
  find_package ( FLEX REQUIRED )
endif ()

if ( USE_TBB )
  find_package ( TBB )
endif ()

if ( NOT USE_INTERNAL_BLASLAPACK )
  find_package ( BLAS )
  find_package ( LAPACK )
endif ()

if ( NOT USE_INTERNAL_MUPARSER )
  find_package ( MuParser 1.32 )
endif ()

find_package ( Doxygen )

if ( USE_LIBXML2 )
  find_package ( LibXml2 )
endif ()

find_package ( Threads )
find_package ( Libdl )
find_package ( Regex )

if ( USE_R )
  find_package ( R COMPONENTS base rot )
endif ()

# Find system prerequisites
include ( CheckIncludeFile )
check_include_file ( arpa/inet.h   HAVE_ARPA_INET_H   )
check_include_file ( assert.h      HAVE_ASSERT_H      )
check_include_file ( dirent.h      HAVE_DIRENT_H      )
check_include_file ( errno.h       HAVE_ERRNO_H       )
check_include_file ( inttypes.h    HAVE_INTTYPES_H    )
check_include_file ( malloc.h      HAVE_MALLOC_H      )
check_include_file ( math.h        HAVE_MATH_H        )
check_include_file ( netdb.h       HAVE_NETDB_H       )
check_include_file ( netinet/in.h  HAVE_NETINET_IN_H  )
check_include_file ( netinet/tcp.h HAVE_NETINET_TCP_H )
check_include_file ( pthread.h     HAVE_PTHREAD_H     )
check_include_file ( regex.h       HAVE_REGEX_H       )
check_include_file ( semaphore.h   HAVE_SEMAPHORE_H   )
check_include_file ( signal.h      HAVE_SIGNAL_H      )
check_include_file ( stdbool.h     HAVE_STDBOOL_H     )
check_include_file ( stdint.h      HAVE_STDINT_H      )
check_include_file ( stdlib.h      HAVE_STDLIB_H      )
check_include_file ( string.h      HAVE_STRING_H      )
check_include_file ( strings.h     HAVE_STRINGS_H     )
check_include_file ( sys/socket.h  HAVE_SYS_SOCKET_H  )
check_include_file ( sys/stat.h    HAVE_SYS_STAT_H    )
check_include_file ( sys/time.h    HAVE_SYS_TIME_H    )
check_include_file ( sys/types.h   HAVE_SYS_TYPES_H   )
check_include_file ( sys/un.h      HAVE_SYS_UN_H      )
check_include_file ( unistd.h      HAVE_UNISTD_H      )


# Some useful macros to ease CMakeLists.txt file writing
set ( SOURCEFILES "" CACHE INTERNAL "List of source files to compile" )
macro ( ot_add_source_file FILENAME )
  set ( sf ${SOURCEFILES} ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME} )
  set ( SOURCEFILES ${sf} CACHE INTERNAL "List of source files to compile" )
endmacro ( ot_add_source_file )

set ( BUILT_SOURCEFILES "" CACHE INTERNAL "List of source files to compile" )
macro ( ot_add_built_source_file FILENAME )
  set ( sf ${BUILT_SOURCEFILES} ${FILENAME} )
  set ( BUILT_SOURCEFILES ${sf} CACHE INTERNAL "List of built source files to compile" )
endmacro ( ot_add_built_source_file )

set ( HEADERFILES "" CACHE INTERNAL "List of header files to install" )
macro ( ot_install_header_file FILENAME )
  set ( hf ${HEADERFILES} ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME} )
  set ( HEADERFILES ${hf} CACHE INTERNAL "List of header files to install" )
endmacro ( ot_install_header_file )

set ( SWIGFILES "" CACHE INTERNAL "List of SWIG files to install" )
macro ( ot_install_swig_file FILENAME )
  set ( hf ${SWIGFILES} ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME} )
  set ( SWIGFILES ${hf} CACHE INTERNAL "List of SWIG files to install" )
endmacro ( ot_install_swig_file )

set ( INTERNAL_INCLUDE_DIRS "" CACHE INTERNAL "List of directories with header files needed for build" )
macro ( ot_add_current_dir_to_include_dirs )
  set ( inc_dirs ${INTERNAL_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} )
  set ( INTERNAL_INCLUDE_DIRS ${inc_dirs} CACHE INTERNAL "List of directories with header files needed for build" )
endmacro ( ot_add_current_dir_to_include_dirs )
macro ( ot_add_build_dir_to_include_dirs DIR )
  set ( inc_dirs ${INTERNAL_INCLUDE_DIRS} ${DIR} )
  set ( INTERNAL_INCLUDE_DIRS ${inc_dirs} CACHE INTERNAL "List of directories with header files needed for build" )
endmacro ( ot_add_build_dir_to_include_dirs )

if ( MINGW )
  set ( WIN32 1 )
endif ( MINGW )

if ( WIN32 )
  list ( APPEND OPENTURNS_DEFINITIONS -DWIN32 )
endif ( WIN32 )

# Add definitions according to the prerequisites found
add_definitions( -DHAVE_CONFIG_H )
if ( MINGW )
  list ( APPEND OPENTURNS_LIBRARIES ws2_32 )
else ( MINGW )
  list ( APPEND OPENTURNS_DEFINITIONS -fPIC -DPIC )
  list ( APPEND OPENTURNS_LIBRARIES ${CMAKE_Fortran_IMPLICIT_LINK_LIBRARIES} )
endif ( MINGW )

# CMake makefiles don't link against stdc++ on OSX
if ( APPLE )
  list ( APPEND OPENTURNS_LIBRARIES stdc++ )
endif ()

if ( LIBXML2_FOUND )
  set ( HAVE_LIBXML2 TRUE )
  list ( APPEND OPENTURNS_DEFINITIONS ${LIBXML2_DEFINITIONS} )
  list ( APPEND OPENTURNS_INCLUDE_DIRS ${LIBXML2_INCLUDE_DIR} )
  list ( APPEND OPENTURNS_LIBRARIES ${LIBXML2_LIBRARIES} )
endif ( LIBXML2_FOUND )

if ( LAPACK_FOUND )
  list ( APPEND OPENTURNS_LIBRARIES ${LAPACK_LIBRARIES} )
endif ( LAPACK_FOUND )

if ( MUPARSER_FOUND )
  list ( APPEND OPENTURNS_LIBRARIES ${MUPARSER_LIBRARIES} )
  list ( APPEND OPENTURNS_INCLUDE_DIRS ${MUPARSER_INCLUDE_DIRS} )
endif ()

if ( MINGW )
  list ( APPEND OPENTURNS_DEFINITIONS -mthreads )
  list ( APPEND OPENTURNS_LIBRARIES pthreadGC2 )
else ( MINGW )
  if ( Threads_FOUND )
    list ( APPEND OPENTURNS_DEFINITIONS -pthread )
    list ( APPEND OPENTURNS_LIBRARIES ${CMAKE_THREAD_LIBS_INIT} )
  endif ( Threads_FOUND )
endif ( MINGW )

if ( TBB_FOUND )
  set ( HAVE_TBB TRUE )
  set ( HAVE_TBB_TBB_H TRUE )
  list ( APPEND OPENTURNS_INCLUDE_DIRS ${TBB_INCLUDE_DIRS} )
  list ( APPEND OPENTURNS_LIBRARIES ${TBB_LIBRARIES} )
endif ( TBB_FOUND )

if ( LIBDL_FOUND )
  set ( HAVE_LIBDL TRUE )
  list ( APPEND OPENTURNS_INCLUDE_DIRS ${LIBDL_INCLUDE_DIR} )
  list ( APPEND OPENTURNS_LIBRARIES ${LIBDL_LIBRARIES} )
endif ( LIBDL_FOUND )

if ( REGEX_FOUND )
  set ( HAVE_REGEX TRUE )
  list ( APPEND OPENTURNS_INCLUDE_DIRS ${REGEX_INCLUDE_DIR} )
  list ( APPEND OPENTURNS_LIBRARIES ${REGEX_LIBRARIES} )
endif ( REGEX_FOUND )

add_definitions ( ${OPENTURNS_DEFINITIONS} )
if ( OPENTURNS_INCLUDE_DIRS )
  list ( REMOVE_DUPLICATES OPENTURNS_INCLUDE_DIRS )
endif ( OPENTURNS_INCLUDE_DIRS )
include_directories ( ${OPENTURNS_INCLUDE_DIRS} )

set ( CPACK_PACKAGE_NAME          openturns      )
set ( CPACK_PACKAGE_VERSION_MAJOR 1              )
set ( CPACK_PACKAGE_VERSION_MINOR 1-based-devel )
set ( CPACK_PACKAGE_VERSION_PATCH                )
set ( CPACK_SOURCE_GENERATOR      "TGZ;TBZ2"     )
set ( CPACK_BINARY_STGZ           "OFF"          )
set ( CPACK_BINARY_TBZ2           "ON"           )
set ( CPACK_BINARY_TGZ            "ON"           )
set ( CPACK_BINARY_TZ             "OFF"          )
set ( CPACK_SOURCE_IGNORE_FILES "/.svn;/build;.*~;${CPACK_SOURCE_IGNORE_FILES}" )
#set ( CPACK_INSTALL_CMAKE_PROJECTS "lib;OpenTURNSLib;ALL;/" "python;OpenTURNSPython;ALL;/" )


# Set global definitions
# TODO: remove PACKAGE_<XX> from source code. Needed for compatibility with autotools
set ( PACKAGE_NAME           ${CPACK_PACKAGE_NAME} )
set ( PACKAGE_VERSION        ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR} )
if ( CPACK_PACKAGE_VERSION_PATCH )
  set ( PACKAGE_VERSION       ${PACKAGE_VERSION}.${CPACK_PACKAGE_VERSION_PATCH} )
endif ( CPACK_PACKAGE_VERSION_PATCH )
set ( PACKAGE_BUGREPORT      bugs@openturns.org )
set( CPACK_SOURCE_PACKAGE_FILE_NAME ${PACKAGE_NAME}-${PACKAGE_VERSION} )

# uninstall target
configure_file (
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
  IMMEDIATE @ONLY
)

add_custom_target ( uninstall
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

# distcheck target
find_program( TAR_PROGRAM tar ) 
get_filename_component( BUILD_TOOL_NAME ${CMAKE_BUILD_TOOL} NAME )
if ( TAR_PROGRAM AND BUILD_TOOL_NAME MATCHES ".*make" )
  set ( DISTCHECK_BINARY_DIR ${CMAKE_BINARY_DIR}/${CPACK_SOURCE_PACKAGE_FILE_NAME}/_build )
  set ( DISTCHECK_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/${CPACK_SOURCE_PACKAGE_FILE_NAME}/_inst )
  add_custom_target ( distcheck 
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR} "$(MAKE)" package_source
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${DISTCHECK_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${DISTCHECK_INSTALL_PREFIX}
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR} ${TAR_PROGRAM} xvfz ${CMAKE_BINARY_DIR}/${CPACK_SOURCE_PACKAGE_FILE_NAME}.tar.gz
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DISTCHECK_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DISTCHECK_INSTALL_PREFIX}
    COMMAND ${CMAKE_COMMAND} -E chdir ${DISTCHECK_BINARY_DIR} ${CMAKE_COMMAND} .. -DCMAKE_INSTALL_PREFIX=${DISTCHECK_INSTALL_PREFIX}
    COMMAND ${CMAKE_COMMAND} -E chdir ${DISTCHECK_BINARY_DIR} "$(MAKE)"
    COMMAND ${CMAKE_COMMAND} -E chdir ${DISTCHECK_BINARY_DIR} "$(MAKE)" check
    COMMAND ${CMAKE_COMMAND} -E chdir ${DISTCHECK_BINARY_DIR} "$(MAKE)" install
    COMMAND ${CMAKE_COMMAND} -E chdir ${DISTCHECK_BINARY_DIR} "$(MAKE)" cppinstallcheck
    COMMAND ${CMAKE_COMMAND} -E chdir ${DISTCHECK_BINARY_DIR} "$(MAKE)" pyinstallcheck
    COMMAND ${CMAKE_COMMAND} -E chdir ${DISTCHECK_BINARY_DIR} "$(MAKE)" uninstall
    COMMAND ${CMAKE_COMMAND} -E chdir ${DISTCHECK_BINARY_DIR} "$(MAKE)" clean
  )
endif ()

set ( OPENTURNS_HOME_ENV_VAR OPENTURNS_HOME     )
set ( OPENTURNS_INSTALL_PATH
      ${CMAKE_INSTALL_PREFIX}
      CACHE PATH "The directory where Open TURNS is installed"
    )  
    
if ( NOT DEFINED CMAKE_INSTALL_LIBDIR )
  if ( ${CMAKE_VERSION} VERSION_GREATER 2.8.7 )
    # Use GNUInstallDirs in order to enforce lib64 if needed
    # but only handles multiarch from 2.8.8
    include( GNUInstallDirs OPTIONAL RESULT_VARIABLE GNUInstallDirsModuleFound )
  endif ()
  if ( NOT GNUInstallDirsModuleFound )
    set(_LIBDIR_DEFAULT "lib")
    # Override this default 'lib' with 'lib64' iff:
    #  - we are on Linux system but NOT cross-compiling
    #  - we are NOT on debian
    #  - we are on a 64 bits system
    # reason is: amd64 ABI: http://www.x86-64.org/documentation/abi.pdf
    # For Debian with multiarch, use 'lib/${CMAKE_LIBRARY_ARCHITECTURE}' if
    # CMAKE_LIBRARY_ARCHITECTURE is set (which contains e.g. "i386-linux-gnu"
    # See http://wiki.debian.org/Multiarch
    if(CMAKE_SYSTEM_NAME MATCHES "Linux"
        AND NOT CMAKE_CROSSCOMPILING)
      if (EXISTS "/etc/debian_version") # is this a debian system ?
        if(CMAKE_LIBRARY_ARCHITECTURE)
          set(_LIBDIR_DEFAULT "lib/${CMAKE_LIBRARY_ARCHITECTURE}")
        endif()
      else() # not debian, rely on CMAKE_SIZEOF_VOID_P:
        if(NOT DEFINED CMAKE_SIZEOF_VOID_P)
          message(AUTHOR_WARNING
            "Unable to determine default CMAKE_INSTALL_LIBDIR directory because no target architecture is known. "
            "Please enable at least one language before including GNUInstallDirs.")
        else()
          if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
            set(_LIBDIR_DEFAULT "lib64")
          endif()
        endif()
      endif()
    endif()
    set(CMAKE_INSTALL_LIBDIR "${_LIBDIR_DEFAULT}" CACHE PATH "object code libraries (${_LIBDIR_DEFAULT})")
  endif ( NOT GNUInstallDirsModuleFound )
endif ( NOT DEFINED CMAKE_INSTALL_LIBDIR )

set ( OPENTURNS_LIBRARY_PATH
      ${CMAKE_INSTALL_LIBDIR}/openturns
      CACHE PATH "The directory where the library is installed"
    )
set ( OPENTURNS_INCLUDE_PATH
      include/openturns
      CACHE PATH "The directory where the header files are installed"
    )
set ( OPENTURNS_CONFIG_CMAKE_PATH
      ${CMAKE_INSTALL_LIBDIR}/openturns
      CACHE PATH "The directory where the CMake files are installed"
    )
set ( OPENTURNS_SYSCONFIG_PATH
      etc
      CACHE PATH "The directory where the configuration file is installed"
    )
set ( OPENTURNS_DATA_PATH
      share
      CACHE PATH "The directory where the common files are installed"
    )
set ( OPENTURNS_WRAPPER_PATH
      ${CMAKE_INSTALL_LIBDIR}/openturns/wrappers
      CACHE PATH "The directory where the wrapper files are installed"
    )
set ( OPENTURNS_EXAMPLE_PATH
      share/openturns/examples
      CACHE PATH "The directory where the example files are installed"
    )
set ( OPENTURNS_SAMPLE_PATH
      share/openturns/examples
      CACHE PATH "The directory where the sample files are installed"
    )
set ( OPENTURNS_VALIDATION_PATH
      share/openturns/validation
      CACHE PATH "The directory where validation files are installed"
    )
set ( OPENTURNS_DOC_PATH
      share/openturns/doc
      CACHE PATH "The directory where the license files are installed"
    )

if ( MINGW )
  set ( DEFAULT_TMP TEMP )
else ( MINGW )
  set( DEFAULT_TMP /tmp )
endif( MINGW ) 

set ( OPENTURNS_TEMPDIR
      ${DEFAULT_TMP}
      CACHE PATH "The directory for temporary files. /tmp by default."
    )
set ( TEMPDIR ${OPENTURNS_TEMPDIR} )

# WARNING: This is where variables without OPENTURNS_ prefix are defined !
foreach ( _var INSTALL_PATH LIBRARY_PATH INCLUDE_PATH CONFIG_CMAKE_PATH SYSCONFIG_PATH DATA_PATH WRAPPER_PATH EXAMPLE_PATH SAMPLE_PATH VALIDATION_PATH DOC_PATH )
  if ( IS_ABSOLUTE ${OPENTURNS_${_var}} )
    set(${_var} ${OPENTURNS_${_var}})
  else ( IS_ABSOLUTE ${OPENTURNS_${_var}} )
    set(${_var} ${OPENTURNS_INSTALL_PATH}/${OPENTURNS_${_var}})
  endif ( IS_ABSOLUTE ${OPENTURNS_${_var}} )
endforeach ( _var )

# Selectively add or remove RPATH from executable
if ( NOT OPENTURNS_SYSTEM_INSTALL )
  set ( CMAKE_INSTALL_RPATH ${LIBRARY_PATH}
                            ${CMAKE_Fortran_IMPLICIT_LINK_DIRECTORIES}
      )
  set ( CMAKE_BUILD_WITH_INSTALL_RPATH    TRUE )
  set ( CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE )
endif ( NOT OPENTURNS_SYSTEM_INSTALL )

set ( OPENTURNS_SWIG_INCLUDE_DIRS ${INCLUDE_PATH}/swig )
set ( OPENTURNS_SWIG_DEFINITIONS -DSWIG_TYPE_TABLE=openturns )

# The tests can't be run if this function is absent
enable_testing ()
add_custom_target ( check COMMENT "Run pre-installation tests" )
add_custom_target ( installcheck COMMENT "Run post-installation tests" )

# Recurse in subdirectories
add_subdirectory ( utils )
add_subdirectory ( wrappers )

if ( OPENTURNS_BUILD_PYTHON )
  find_package ( SWIG )
  if ( SWIG_FOUND )
    include ( ${SWIG_USE_FILE} )
    mark_as_advanced ( SWIG_VERSION SWIG_DIR )
  endif ()
  find_package ( PythonInterp )
  find_package ( PythonLibs )
  include ( FindPythonModule )
  find_python_module ( numpy )
  find_python_module ( scipy )
  find_python_module ( matplotlib 1.1 )

  if ( PYTHONINTERP_FOUND )
    execute_process ( COMMAND ${PYTHON_EXECUTABLE} -c "import sys; print( '%d.%d' % ( sys.version_info[0], sys.version_info[1] ) )"
                      OUTPUT_VARIABLE PYTHON_VERSION
                      OUTPUT_STRIP_TRAILING_WHITESPACE )

    if ( MINGW )
      string ( REPLACE "." "" PYTHON_VERSION "${PYTHON_VERSION}" )
    endif ( MINGW )

    # guess LIB_SUFFIX to determine python lib dir
    # do not use CMAKE_INSTALL_LIBDIR to ignore multiarch
    if ( NOT DEFINED LIB_SUFFIX )
      if ( CMAKE_SYSTEM_NAME MATCHES "Linux" AND NOT CMAKE_CROSSCOMPILING )
        if ( NOT EXISTS "/etc/debian_version" )
          if ( "${CMAKE_SIZEOF_VOID_P}" EQUAL "8" )
            set ( LIB_SUFFIX 64 )
          endif ()
        endif ()
      endif ()
    endif ()
    
    set ( OPENTURNS_PYTHON_MODULE_PATH
          lib${LIB_SUFFIX}/python${PYTHON_VERSION}${SITE_SUBDIR}
          CACHE PATH "The directory where the python modules are installed"
        )

    if ( IS_ABSOLUTE ${OPENTURNS_PYTHON_MODULE_PATH} )
      set( PYTHON_MODULE_PATH ${OPENTURNS_PYTHON_MODULE_PATH} )
    else ( IS_ABSOLUTE ${OPENTURNS_PYTHON_MODULE_PATH} )
      set( PYTHON_MODULE_PATH ${OPENTURNS_INSTALL_PATH}/${OPENTURNS_PYTHON_MODULE_PATH} )
    endif ( IS_ABSOLUTE ${OPENTURNS_PYTHON_MODULE_PATH} )

  endif ( PYTHONINTERP_FOUND )
  
endif ( OPENTURNS_BUILD_PYTHON )


if ( OPENTURNS_BUILD_LIB )
  add_subdirectory ( lib )
  add_dependencies ( check cppcheck )
  add_dependencies ( installcheck cppinstallcheck )
endif ( OPENTURNS_BUILD_LIB )

if ( PYTHONINTERP_FOUND AND PYTHONLIBS_FOUND AND SWIG_FOUND  )
  add_subdirectory ( python )
  add_dependencies ( installcheck pyinstallcheck )
endif ()


if ( OPENTURNS_BUILD_VALIDATION )
  add_subdirectory ( validation )
endif ( OPENTURNS_BUILD_VALIDATION )

install ( FILES ${HEADERFILES}
          DESTINATION ${INCLUDE_PATH}
          PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
        )

include ( CPack )
