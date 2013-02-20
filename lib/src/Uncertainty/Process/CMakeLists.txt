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

ot_add_source_file ( WhiteNoise.cxx )
ot_add_source_file ( RandomWalk.cxx )
ot_add_source_file ( ARMACoefficients.cxx )
ot_add_source_file ( ARMAState.cxx )
ot_add_source_file ( ARMA.cxx )
ot_add_source_file ( SpectralNormalProcess.cxx )
ot_add_source_file ( TemporalNormalProcess.cxx )
ot_add_source_file ( CompositeProcess.cxx )
ot_add_source_file ( FunctionalBasisProcess.cxx )
ot_add_source_file ( ARMAFactoryImplementation.cxx )
ot_add_source_file ( ARMAFactory.cxx )
ot_add_source_file ( WhittleFactory.cxx )
ot_add_source_file ( WhittleFactoryState.cxx )
ot_add_source_file ( ARMALikelihoodFactory.cxx )

ot_install_header_file ( OTProcess.hxx )
ot_install_header_file ( WhiteNoise.hxx )
ot_install_header_file ( RandomWalk.hxx )
ot_install_header_file ( ARMACoefficients.hxx )
ot_install_header_file ( ARMAState.hxx )
ot_install_header_file ( ARMA.hxx )
ot_install_header_file ( SpectralNormalProcess.hxx )
ot_install_header_file ( TemporalNormalProcess.hxx )
ot_install_header_file ( CompositeProcess.hxx )
ot_install_header_file ( FunctionalBasisProcess.hxx )
ot_install_header_file ( ARMAFactoryImplementation.hxx )
ot_install_header_file ( ARMAFactory.hxx )
ot_install_header_file ( WhittleFactory.hxx )
ot_install_header_file ( WhittleFactoryState.hxx )
ot_install_header_file ( ARMALikelihoodFactory.hxx )
