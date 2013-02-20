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

ot_add_source_file ( SimulationResultImplementation.cxx )
ot_add_source_file ( SimulationResult.cxx )
ot_add_source_file ( SimulationSensitivityAnalysis.cxx )
ot_add_source_file ( PostAnalyticalSimulation.cxx )
ot_add_source_file ( Simulation.cxx )
ot_add_source_file ( DirectionalSampling.cxx )
ot_add_source_file ( RootStrategy.cxx )
ot_add_source_file ( RootStrategyImplementation.cxx )
ot_add_source_file ( RiskyAndFast.cxx )
ot_add_source_file ( MediumSafe.cxx )
ot_add_source_file ( SafeAndSlow.cxx )
ot_add_source_file ( SamplingStrategy.cxx )
ot_add_source_file ( SamplingStrategyImplementation.cxx )
ot_add_source_file ( RandomDirection.cxx )
ot_add_source_file ( OrthogonalDirection.cxx )
ot_add_source_file ( ImportanceSampling.cxx )
ot_add_source_file ( LHS.cxx )
ot_add_source_file ( MonteCarlo.cxx )
ot_add_source_file ( PostAnalyticalControlledImportanceSampling.cxx )
ot_add_source_file ( PostAnalyticalImportanceSampling.cxx )
ot_add_source_file ( QuasiMonteCarloResult.cxx )
ot_add_source_file ( QuasiMonteCarlo.cxx )
ot_add_source_file ( RandomizedQuasiMonteCarlo.cxx )
ot_add_source_file ( RandomizedLHS.cxx )
ot_add_source_file ( Wilks.cxx )

ot_install_header_file ( Wilks.hxx )
ot_install_header_file ( RootStrategyImplementation.hxx )
ot_install_header_file ( SamplingStrategy.hxx )
ot_install_header_file ( PostAnalyticalControlledImportanceSampling.hxx )
ot_install_header_file ( QuasiMonteCarloResult.hxx )
ot_install_header_file ( SimulationResult.hxx )
ot_install_header_file ( RiskyAndFast.hxx )
ot_install_header_file ( RandomDirection.hxx )
ot_install_header_file ( OrthogonalDirection.hxx )
ot_install_header_file ( PostAnalyticalImportanceSampling.hxx )
ot_install_header_file ( ImportanceSampling.hxx )
ot_install_header_file ( SimulationResultImplementation.hxx )
ot_install_header_file ( SimulationSensitivityAnalysis.hxx )
ot_install_header_file ( DirectionalSampling.hxx )
ot_install_header_file ( RootStrategy.hxx )
ot_install_header_file ( SafeAndSlow.hxx )
ot_install_header_file ( SamplingStrategyImplementation.hxx )
ot_install_header_file ( OTSimulation.hxx )
ot_install_header_file ( QuasiMonteCarlo.hxx )
ot_install_header_file ( RandomizedQuasiMonteCarlo.hxx )
ot_install_header_file ( PostAnalyticalSimulation.hxx )
ot_install_header_file ( MediumSafe.hxx )
ot_install_header_file ( Simulation.hxx )
ot_install_header_file ( MonteCarlo.hxx )
ot_install_header_file ( LHS.hxx )
ot_install_header_file ( RandomizedLHS.hxx )
