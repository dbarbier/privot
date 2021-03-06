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

ot_add_source_file ( AnalyticalParser.cxx )
ot_add_source_file ( AnalyticalNumericalMathEvaluationImplementation.cxx )
ot_add_source_file ( AnalyticalNumericalMathGradientImplementation.cxx )
ot_add_source_file ( AnalyticalNumericalMathHessianImplementation.cxx )
ot_add_source_file ( Library.cxx )
ot_add_source_file ( LibraryLoader.cxx )
ot_add_source_file ( WrapperData.cxx )
ot_add_source_file ( WrapperFile.cxx )
ot_add_source_file ( WrapperFile_version1.cxx )
ot_add_source_file ( WrapperFile_version2.cxx )
ot_add_source_file ( WrapperObject.cxx )
ot_add_source_file ( DynamicalFunction.cxx )
ot_add_source_file ( DynamicalFunctionImplementation.cxx )
ot_add_source_file ( SpatialFunction.cxx )
ot_add_source_file ( TemporalFunction.cxx )
ot_add_source_file ( NumericalMathFunction.cxx )
ot_add_source_file ( NumericalMathFunctionImplementation.cxx )
ot_add_source_file ( ComposedNumericalMathFunction.cxx )
ot_add_source_file ( LinearNumericalMathFunction.cxx )
ot_add_source_file ( NumericalMathEvaluationImplementation.cxx )
ot_add_source_file ( NumericalMathGradientImplementation.cxx )
ot_add_source_file ( NumericalMathHessianImplementation.cxx )
ot_add_source_file ( NoNumericalMathEvaluationImplementation.cxx )
ot_add_source_file ( NoNumericalMathGradientImplementation.cxx )
ot_add_source_file ( NoNumericalMathHessianImplementation.cxx )
ot_add_source_file ( ComputedNumericalMathEvaluationImplementation.cxx )
ot_add_source_file ( ComputedNumericalMathGradientImplementation.cxx )
ot_add_source_file ( ComputedNumericalMathHessianImplementation.cxx )
ot_add_source_file ( LinearNumericalMathEvaluationImplementation.cxx )
ot_add_source_file ( QuadraticNumericalMathEvaluationImplementation.cxx )
ot_add_source_file ( LinearNumericalMathGradientImplementation.cxx )
ot_add_source_file ( ConstantNumericalMathGradientImplementation.cxx )
ot_add_source_file ( ConstantNumericalMathHessianImplementation.cxx )
ot_add_source_file ( ComposedNumericalMathEvaluationImplementation.cxx )
ot_add_source_file ( ComposedNumericalMathGradientImplementation.cxx )
ot_add_source_file ( ComposedNumericalMathHessianImplementation.cxx )
ot_add_source_file ( IndicatorNumericalMathEvaluationImplementation.cxx )
ot_add_source_file ( UniVariatePolynomialImplementation.cxx )
ot_add_source_file ( UniVariatePolynomial.cxx )
ot_add_source_file ( ProductPolynomialEvaluationImplementation.cxx )
ot_add_source_file ( RestrictedEvaluationImplementation.cxx )
ot_add_source_file ( RestrictedGradientImplementation.cxx )
ot_add_source_file ( RestrictedHessianImplementation.cxx )
ot_add_source_file ( DualLinearCombinationEvaluationImplementation.cxx )
ot_add_source_file ( DualLinearCombinationGradientImplementation.cxx )
ot_add_source_file ( DualLinearCombinationHessianImplementation.cxx )
ot_add_source_file ( ExpertMixture.cxx )
ot_add_source_file ( LinearCombinationEvaluationImplementation.cxx )
ot_add_source_file ( LinearCombinationGradientImplementation.cxx )
ot_add_source_file ( LinearCombinationHessianImplementation.cxx )
ot_add_source_file ( ProductNumericalMathEvaluationImplementation.cxx )
ot_add_source_file ( ProductNumericalMathFunction.cxx )
ot_add_source_file ( ProductNumericalMathGradientImplementation.cxx )
ot_add_source_file ( ProductNumericalMathHessianImplementation.cxx )
ot_add_source_file ( DatabaseNumericalMathEvaluationImplementation.cxx )
ot_add_source_file ( AggregatedNumericalMathEvaluationImplementation.cxx)
ot_add_source_file ( ProductPolynomialHessianImplementation.cxx )
ot_add_source_file ( ProductPolynomialGradientImplementation.cxx )
ot_add_source_file ( Basis.cxx )
ot_add_source_file ( BasisSequenceImplementation.cxx )
ot_add_source_file ( BasisSequence.cxx )
ot_add_source_file ( BasisSequenceFactoryImplementation.cxx )
ot_add_source_file ( BasisSequenceFactory.cxx )
ot_add_source_file ( LAR.cxx )
ot_add_source_file ( LARLasso.cxx )
ot_add_source_file ( LARFSZero.cxx )
ot_add_source_file ( BoxCoxEvaluationImplementation.cxx )
ot_add_source_file ( BoxCoxTransform.cxx )
ot_add_source_file ( InverseBoxCoxEvaluationImplementation.cxx )
ot_add_source_file ( InverseBoxCoxTransform.cxx )
ot_add_source_file ( TrendEvaluationImplementation.cxx )
ot_add_source_file ( TrendTransform.cxx )
ot_add_source_file ( InverseTrendEvaluationImplementation.cxx )
ot_add_source_file ( InverseTrendTransform.cxx )

ot_install_header_file ( AnalyticalParser.hxx )
ot_install_header_file ( AnalyticalNumericalMathEvaluationImplementation.hxx )
ot_install_header_file ( AnalyticalNumericalMathGradientImplementation.hxx )
ot_install_header_file ( AnalyticalNumericalMathHessianImplementation.hxx )
ot_install_header_file ( ProductPolynomialGradientImplementation.hxx )
ot_install_header_file ( ProductPolynomialHessianImplementation.hxx )
ot_install_header_file ( AggregatedNumericalMathEvaluationImplementation.hxx)
ot_install_header_file ( ConstantNumericalMathGradientImplementation.hxx )
ot_install_header_file ( ProductPolynomialEvaluationImplementation.hxx )
ot_install_header_file ( QuadraticNumericalMathEvaluationImplementation.hxx )
ot_install_header_file ( NoNumericalMathGradientImplementation.hxx )
ot_install_header_file ( ProductNumericalMathEvaluationImplementation.hxx )
ot_install_header_file ( NoNumericalMathHessianImplementation.hxx )
ot_install_header_file ( IndicatorNumericalMathEvaluationImplementation.hxx )
ot_install_header_file ( MethodBoundNumericalMathEvaluationImplementation.hxx )
ot_install_header_file ( WrapperObject.hxx )
ot_install_header_file ( ComposedNumericalMathHessianImplementation.hxx )
ot_install_header_file ( OTFunc.hxx )
ot_install_header_file ( NumericalMathHessianImplementation.hxx )
ot_install_header_file ( WrapperFile.hxx )
ot_install_header_file ( WrapperFile_version1.hxx )
ot_install_header_file ( WrapperFile_version2.hxx )
ot_install_header_file ( ProductNumericalMathFunction.hxx )
ot_install_header_file ( LinearNumericalMathGradientImplementation.hxx )
ot_install_header_file ( LinearNumericalMathEvaluationImplementation.hxx )
ot_install_header_file ( ComputedNumericalMathHessianImplementation.hxx )
ot_install_header_file ( ComputedNumericalMathGradientImplementation.hxx )
ot_install_header_file ( XMLTags.hxx )
ot_install_header_file ( WrapperData.hxx )
ot_install_header_file ( NoNumericalMathEvaluationImplementation.hxx )
ot_install_header_file ( Library.hxx )
ot_install_header_file ( ConstantNumericalMathHessianImplementation.hxx )
ot_install_header_file ( LinearNumericalMathFunction.hxx )
ot_install_header_file ( UniVariatePolynomial.hxx )
ot_install_header_file ( ProductNumericalMathGradientImplementation.hxx )
ot_install_header_file ( LibraryLoader.hxx )
ot_install_header_file ( ComposedNumericalMathEvaluationImplementation.hxx )
ot_install_header_file ( NumericalMathGradientImplementation.hxx )
ot_install_header_file ( NumericalMathEvaluationImplementation.hxx )
ot_install_header_file ( ComputedNumericalMathEvaluationImplementation.hxx )
ot_install_header_file ( ComposedNumericalMathGradientImplementation.hxx )
ot_install_header_file ( RestrictedEvaluationImplementation.hxx )
ot_install_header_file ( RestrictedGradientImplementation.hxx )
ot_install_header_file ( RestrictedHessianImplementation.hxx )
ot_install_header_file ( DualLinearCombinationEvaluationImplementation.hxx )
ot_install_header_file ( DualLinearCombinationGradientImplementation.hxx )
ot_install_header_file ( DualLinearCombinationHessianImplementation.hxx )
ot_install_header_file ( ExpertMixture.hxx )
ot_install_header_file ( LinearCombinationEvaluationImplementation.hxx )
ot_install_header_file ( LinearCombinationGradientImplementation.hxx )
ot_install_header_file ( LinearCombinationHessianImplementation.hxx )
ot_install_header_file ( ComposedNumericalMathFunction.hxx )
ot_install_header_file ( UniVariatePolynomialImplementation.hxx )
ot_install_header_file ( NumericalMathFunctionImplementation.hxx )
ot_install_header_file ( NumericalMathFunction.hxx )
ot_install_header_file ( DynamicalFunctionImplementation.hxx )
ot_install_header_file ( DynamicalFunction.hxx )
ot_install_header_file ( SpatialFunction.hxx )
ot_install_header_file ( TemporalFunction.hxx )
ot_install_header_file ( ProductNumericalMathHessianImplementation.hxx )
ot_install_header_file ( DatabaseNumericalMathEvaluationImplementation.hxx )
ot_install_header_file ( Basis.hxx )
ot_install_header_file ( BasisSequenceImplementation.hxx )
ot_install_header_file ( BasisSequence.hxx )
ot_install_header_file ( BasisSequenceFactoryImplementation.hxx )
ot_install_header_file ( BasisSequenceFactory.hxx )
ot_install_header_file ( LAR.hxx )
ot_install_header_file ( LARLasso.hxx )
ot_install_header_file ( LARFSZero.hxx )
ot_install_header_file ( BoxCoxEvaluationImplementation.hxx )
ot_install_header_file ( BoxCoxTransform.hxx )
ot_install_header_file ( InverseBoxCoxEvaluationImplementation.hxx )
ot_install_header_file ( InverseBoxCoxTransform.hxx )
ot_install_header_file ( TrendEvaluationImplementation.hxx )
ot_install_header_file ( TrendTransform.hxx )
ot_install_header_file ( InverseTrendEvaluationImplementation.hxx )
ot_install_header_file ( InverseTrendTransform.hxx )

# Recurse in subdirectories
if ( NOT MUPARSER_FOUND )
  add_subdirectory ( MuParser )
endif ()
add_subdirectory ( SpecFunc )

install ( FILES wrapper.dtd wrapper-v2.dtd
          DESTINATION ${OPENTURNS_WRAPPER_PATH}
          PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
        )
