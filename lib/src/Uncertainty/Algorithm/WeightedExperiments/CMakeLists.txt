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

ot_add_source_file ( WeightedExperiment.cxx )
ot_add_source_file ( FixedExperiment.cxx )
ot_add_source_file ( GaussProductExperiment.cxx )
ot_add_source_file ( BootstrapExperiment.cxx )
ot_add_source_file ( MonteCarloExperiment.cxx )
ot_add_source_file ( ImportanceSamplingExperiment.cxx )
ot_add_source_file ( LHSExperiment.cxx )
ot_add_source_file ( LowDiscrepancyExperiment.cxx )

ot_install_header_file ( OTWeightedExperiments.hxx )
ot_install_header_file ( WeightedExperiment.hxx )
ot_install_header_file ( FixedExperiment.hxx )
ot_install_header_file ( GaussProductExperiment.hxx )
ot_install_header_file ( BootstrapExperiment.hxx )
ot_install_header_file ( MonteCarloExperiment.hxx )
ot_install_header_file ( ImportanceSamplingExperiment.hxx )
ot_install_header_file ( LHSExperiment.hxx )
ot_install_header_file ( LowDiscrepancyExperiment.hxx )

