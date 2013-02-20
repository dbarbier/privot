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

ot_add_source_file ( NearestPointAlgorithm.cxx )
ot_add_source_file ( NearestPointAlgorithmImplementation.cxx )
ot_add_source_file ( NearestPointAlgorithmImplementationResult.cxx )
ot_add_source_file ( NearestPointChecker.cxx )
ot_add_source_file ( NearestPointCheckerResult.cxx )
ot_add_source_file ( BoundConstrainedAlgorithm.cxx )
ot_add_source_file ( BoundConstrainedAlgorithmImplementation.cxx )
ot_add_source_file ( BoundConstrainedAlgorithmImplementationResult.cxx )

ot_install_header_file ( BoundConstrainedAlgorithm.hxx )
ot_install_header_file ( NearestPointAlgorithm.hxx )
ot_install_header_file ( NearestPointAlgorithmImplementation.hxx )
ot_install_header_file ( NearestPointAlgorithmImplementationResult.hxx )
ot_install_header_file ( OTOptim.hxx )
ot_install_header_file ( BoundConstrainedAlgorithmImplementationResult.hxx )
ot_install_header_file ( BoundConstrainedAlgorithmImplementation.hxx )
ot_install_header_file ( NearestPointChecker.hxx )
ot_install_header_file ( NearestPointCheckerResult.hxx )

# Recurse in subdirectories
add_subdirectory ( Cobyla )
add_subdirectory ( AbdoRackwitz )
add_subdirectory ( TNC )
add_subdirectory ( SQP )
