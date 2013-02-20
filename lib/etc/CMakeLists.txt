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

# TODO: These variables are needed for autotools compatibility
set ( r_prog_path ${R_EXECUTABLE} )
set ( temporary_directory ${TEMPDIR} )

configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/openturns.conf.in
                 ${CMAKE_CURRENT_BINARY_DIR}/openturns.conf
                 @ONLY )

install ( FILES ${CMAKE_CURRENT_BINARY_DIR}/openturns.conf
          DESTINATION ${OPENTURNS_SYSCONFIG_PATH}/openturns
          PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
        )
