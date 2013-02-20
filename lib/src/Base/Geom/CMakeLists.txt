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

ot_add_source_file ( DomainImplementation.cxx )
ot_add_source_file ( Domain.cxx )
ot_add_source_file ( Interval.cxx )
ot_add_source_file ( Mesh.cxx )

ot_install_header_file ( OTGeom.hxx )
ot_install_header_file ( DomainImplementation.hxx )
ot_install_header_file ( Domain.hxx )
ot_install_header_file ( Interval.hxx )
ot_install_header_file ( Mesh.hxx )
