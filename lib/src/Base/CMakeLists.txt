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
ot_install_header_file ( OTBase.hxx )

# Recurse in subdirectories
add_subdirectory ( Algo )
add_subdirectory ( Common )
add_subdirectory ( Diff )
add_subdirectory ( Experiments )
add_subdirectory ( Func )
add_subdirectory ( Geom )
add_subdirectory ( Graph )
add_subdirectory ( MetaModel )
add_subdirectory ( Optim )
add_subdirectory ( Solver )
add_subdirectory ( Stat )
add_subdirectory ( Type )
