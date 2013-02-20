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

ot_add_source_file ( Experiment.cxx )
ot_add_source_file ( ExperimentImplementation.cxx )
ot_add_source_file ( StratifiedExperiment.cxx )
ot_add_source_file ( Axial.cxx )
ot_add_source_file ( Box.cxx )
ot_add_source_file ( Composite.cxx )
ot_add_source_file ( Factorial.cxx )

ot_install_header_file ( OTExperiments.hxx )
ot_install_header_file ( Experiment.hxx )
ot_install_header_file ( ExperimentImplementation.hxx )
ot_install_header_file ( StratifiedExperiment.hxx )
ot_install_header_file ( Axial.hxx )
ot_install_header_file ( Box.hxx )
ot_install_header_file ( Composite.hxx )
ot_install_header_file ( Factorial.hxx )

