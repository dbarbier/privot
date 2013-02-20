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

ot_add_source_file ( AdaptiveStrategy.cxx )
ot_add_source_file ( AdaptiveStrategyImplementation.cxx )
ot_add_source_file ( FixedStrategy.cxx )
ot_add_source_file ( FunctionalChaosAlgorithm.cxx )
ot_add_source_file ( FunctionalChaosResult.cxx )
ot_add_source_file ( ProjectionStrategy.cxx )
ot_add_source_file ( ProjectionStrategyImplementation.cxx )
ot_add_source_file ( LeastSquaresStrategy.cxx )
ot_add_source_file ( IntegrationStrategy.cxx )
ot_add_source_file ( SequentialStrategy.cxx )
ot_add_source_file ( CleaningStrategy.cxx )

ot_install_header_file ( SequentialStrategy.hxx )
ot_install_header_file ( IntegrationStrategy.hxx )
ot_install_header_file ( FixedStrategy.hxx )
ot_install_header_file ( ProjectionStrategyImplementation.hxx )
ot_install_header_file ( LeastSquaresStrategy.hxx )
ot_install_header_file ( FunctionalChaosAlgorithm.hxx )
ot_install_header_file ( OTFunctionalChaos.hxx )
ot_install_header_file ( AdaptiveStrategyImplementation.hxx )
ot_install_header_file ( ProjectionStrategy.hxx )
ot_install_header_file ( CleaningStrategy.hxx )
ot_install_header_file ( FunctionalChaosResult.hxx )
ot_install_header_file ( AdaptiveStrategy.hxx )
