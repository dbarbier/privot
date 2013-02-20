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

ot_add_source_file ( HistoryStrategy.cxx )
ot_add_source_file ( HistoryStrategyImplementation.cxx )
ot_add_source_file ( Compact.cxx )
ot_add_source_file ( Full.cxx )
ot_add_source_file ( Last.cxx )
ot_add_source_file ( Null.cxx )
ot_add_source_file ( NumericalSample.cxx )
ot_add_source_file ( NumericalSampleImplementation.cxx )
ot_add_source_file ( ProcessSample.cxx )
ot_add_source_file ( CovarianceMatrix.cxx )
ot_add_source_file ( CorrelationMatrix.cxx )
ot_add_source_file ( ConfidenceInterval.cxx )
ot_add_source_file ( TestResult.cxx )
ot_add_source_file ( LinearModelFactory.cxx )
ot_add_source_file ( LinearModel.cxx )
ot_add_source_file ( CorrelationAnalysis.cxx )
ot_add_source_file ( SensitivityAnalysis.cxx )
ot_add_source_file ( LowDiscrepancySequenceImplementation.cxx )
ot_add_source_file ( LowDiscrepancySequence.cxx )
ot_add_source_file ( SobolSequence.cxx )
ot_add_source_file ( FaureSequence.cxx )
ot_add_source_file ( HaltonSequence.cxx )
ot_add_source_file ( HaselgroveSequence.cxx )
ot_add_source_file ( ReverseHaltonSequence.cxx )
ot_add_source_file ( RandomGenerator.cxx )
ot_add_source_file ( dsfmt.cxx )
ot_add_source_file ( TimeSeriesImplementation.cxx )
ot_add_source_file ( TimeSeries.cxx )
ot_add_source_file ( SecondOrderModelImplementation.cxx )
ot_add_source_file ( SecondOrderModel.cxx )
ot_add_source_file ( ExponentialCauchy.cxx )
ot_add_source_file ( FilteringWindowsImplementation.cxx )
ot_add_source_file ( FilteringWindows.cxx )
ot_add_source_file ( Hamming.cxx )
ot_add_source_file ( Hanning.cxx )
ot_add_source_file ( CovarianceModelImplementation.cxx )
ot_add_source_file ( SpectralModelImplementation.cxx )
ot_add_source_file ( SpectralModel.cxx )
ot_add_source_file ( CovarianceModel.cxx )
ot_add_source_file ( StationaryCovarianceModel.cxx )
ot_add_source_file ( SpectralModelFactoryImplementation.cxx )
ot_add_source_file ( SpectralModelFactory.cxx )
ot_add_source_file ( UserDefinedSpectralModel.cxx)
ot_add_source_file ( WelchFactory.cxx)
ot_add_source_file ( ExponentialModel.cxx )
ot_add_source_file ( CauchyModel.cxx )
ot_add_source_file ( CovarianceModelFactoryImplementation.cxx )
ot_add_source_file ( CovarianceModelFactory.cxx )
ot_add_source_file ( UserDefinedCovarianceModel.cxx )
ot_add_source_file ( NonStationaryCovarianceModelFactory.cxx )
ot_add_source_file ( UserDefinedStationaryCovarianceModel.cxx )
ot_add_source_file ( StationaryCovarianceModelFactory.cxx )
ot_add_source_file ( kendall.c )

ot_install_header_file ( HistoryStrategyImplementation.hxx )
ot_install_header_file ( SobolSequence.hxx )
ot_install_header_file ( CovarianceMatrix.hxx )
ot_install_header_file ( LinearModelFactory.hxx )
ot_install_header_file ( TestResult.hxx )
ot_install_header_file ( FaureSequence.hxx )
ot_install_header_file ( HaltonSequence.hxx )
ot_install_header_file ( HaselgroveSequence.hxx )
ot_install_header_file ( ReverseHaltonSequence.hxx )
ot_install_header_file ( CorrelationMatrix.hxx )
ot_install_header_file ( Last.hxx )
ot_install_header_file ( ConfidenceInterval.hxx )
ot_install_header_file ( RandomGenerator.hxx )
ot_install_header_file ( csv_parser_state.hxx )
ot_install_header_file ( LinearModel.hxx )
ot_install_header_file ( HistoryStrategy.hxx )
ot_install_header_file ( CorrelationAnalysis.hxx )
ot_install_header_file ( LowDiscrepancySequence.hxx )
ot_install_header_file ( Full.hxx )
ot_install_header_file ( simd.h )
ot_install_header_file ( NumericalSampleImplementation.hxx )
ot_install_header_file ( NumericalSample.hxx )
ot_install_header_file ( ProcessSample.hxx )
ot_install_header_file ( SensitivityAnalysis.hxx )
ot_install_header_file ( OTStat.hxx )
ot_install_header_file ( Compact.hxx )
ot_install_header_file ( LowDiscrepancySequenceImplementation.hxx )
ot_install_header_file ( Null.hxx )
ot_install_header_file ( dsfmt.h )
ot_install_header_file ( TimeSeriesImplementation.hxx )
ot_install_header_file ( TimeSeries.hxx )
ot_install_header_file ( SecondOrderModelImplementation.hxx )
ot_install_header_file ( SecondOrderModel.hxx )
ot_install_header_file ( ExponentialCauchy.hxx )
ot_install_header_file ( FilteringWindowsImplementation.hxx )
ot_install_header_file ( FilteringWindows.hxx )
ot_install_header_file ( Hamming.hxx )
ot_install_header_file ( Hanning.hxx )
ot_install_header_file( CovarianceModelImplementation.hxx )
ot_install_header_file( SpectralModelImplementation.hxx )
ot_install_header_file( SpectralModel.hxx )
ot_install_header_file( CovarianceModel.hxx )
ot_install_header_file( StationaryCovarianceModel.hxx )
ot_install_header_file( SpectralModelFactoryImplementation.hxx )
ot_install_header_file( SpectralModelFactory.hxx )
ot_install_header_file( UserDefinedSpectralModel.hxx )
ot_install_header_file( WelchFactory.hxx )
ot_install_header_file( ExponentialModel.hxx )
ot_install_header_file( CauchyModel.hxx )
ot_install_header_file( CovarianceModelFactoryImplementation.hxx )
ot_install_header_file( CovarianceModelFactory.hxx )
ot_install_header_file( UserDefinedCovarianceModel.hxx )
ot_install_header_file( NonStationaryCovarianceModelFactory.hxx )
ot_install_header_file( UserDefinedStationaryCovarianceModel.hxx )
ot_install_header_file( StationaryCovarianceModelFactory.hxx )
ot_install_header_file( kendall.h )

# Build on the fly source files
if ( BISON_FOUND )
  ot_add_build_dir_to_include_dirs ( ${CMAKE_CURRENT_BINARY_DIR} )
  add_custom_command ( OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/csv_parser.cc ${CMAKE_CURRENT_BINARY_DIR}/csv_parser.hh
                       MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/csv_parser.yy
                       COMMAND ${BISON_EXECUTABLE}
		       ARGS    -o ${CMAKE_CURRENT_BINARY_DIR}/csv_parser.cc
		               ${CMAKE_CURRENT_SOURCE_DIR}/csv_parser.yy
		       COMMENT "CSV parser generation"
		     )
  ot_add_built_source_file ( ${CMAKE_CURRENT_BINARY_DIR}/csv_parser.cc )
  add_custom_target ( generate_csv_parser DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/csv_parser.cc )
else ( BISON_FOUND )
  ot_add_source_file ( csv_parser.cc )
endif ( BISON_FOUND )

if ( FLEX_FOUND )
  ot_add_build_dir_to_include_dirs ( ${CMAKE_CURRENT_BINARY_DIR} )
  add_custom_command ( OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/csv_lexer.cc ${CMAKE_CURRENT_BINARY_DIR}/csv_lexer.h
                       MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/csv_lexer.ll
                       COMMAND ${FLEX_EXECUTABLE}
		       ARGS    -o ${CMAKE_CURRENT_BINARY_DIR}/csv_lexer.cc
		               --header-file=${CMAKE_CURRENT_BINARY_DIR}/csv_lexer.h
			       ${CMAKE_CURRENT_SOURCE_DIR}/csv_lexer.ll
		       COMMENT "CSV lexer generation"
		     )
  ot_add_built_source_file ( ${CMAKE_CURRENT_BINARY_DIR}/csv_lexer.cc )
  add_custom_target ( generate_csv_lexer DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/csv_lexer.cc )
else ( FLEX_FOUND )
  ot_add_source_file ( csv_lexer.cc )
endif ( FLEX_FOUND )


