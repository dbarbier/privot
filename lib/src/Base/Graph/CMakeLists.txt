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

ot_add_source_file ( Drawable.cxx )
ot_add_source_file ( DrawableImplementation.cxx )
ot_add_source_file ( Contour.cxx )
ot_add_source_file ( Curve.cxx )
ot_add_source_file ( Cloud.cxx )
ot_add_source_file ( Staircase.cxx )
ot_add_source_file ( BarPlot.cxx )
ot_add_source_file ( Pairs.cxx )
ot_add_source_file ( Pie.cxx )
ot_add_source_file ( Graph.cxx )
ot_add_source_file ( GraphImplementation.cxx )

ot_install_header_file ( Contour.hxx )
ot_install_header_file ( OTGraph.hxx )
ot_install_header_file ( DrawableImplementation.hxx )
ot_install_header_file ( Cloud.hxx )
ot_install_header_file ( Drawable.hxx )
ot_install_header_file ( BarPlot.hxx )
ot_install_header_file ( GraphImplementation.hxx )
ot_install_header_file ( Pairs.hxx )
ot_install_header_file ( Pie.hxx )
ot_install_header_file ( Staircase.hxx )
ot_install_header_file ( Rfunctions.hxx )
ot_install_header_file ( Curve.hxx )
ot_install_header_file ( Graph.hxx )
