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

ot_add_source_file ( StrongMaximumTest.cxx )
ot_add_source_file ( Analytical.cxx )
ot_add_source_file ( AnalyticalResult.cxx )
ot_add_source_file ( FORM.cxx )
ot_add_source_file ( FORMResult.cxx )
ot_add_source_file ( SORM.cxx )
ot_add_source_file ( SORMResult.cxx )

ot_install_header_file ( Analytical.hxx )
ot_install_header_file ( OTAnalytical.hxx )
ot_install_header_file ( SORMResult.hxx )
ot_install_header_file ( StrongMaximumTest.hxx )
ot_install_header_file ( SORM.hxx )
ot_install_header_file ( FORMResult.hxx )
ot_install_header_file ( AnalyticalResult.hxx )
ot_install_header_file ( FORM.hxx )
