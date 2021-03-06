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

cmake_minimum_required ( VERSION 2.8 )

#include ( CTest )

set ( NEW_PATH "$ENV{PATH}:${INSTALL_DESTDIR}${EXAMPLE_PATH}" )

set ( ADAPT_CHECK_TESTS_EXPONENT )
set ( REPLACE_EOL " | cat " )
if ( MINGW )
  # replace dos eols
  set ( REPLACE_EOL " | tr -d '\\\\015' " )
  # By default, Mingw-g++ puts 3 digits to real exponent whereas g++ puts 2.
  set ( ADAPT_CHECK_TESTS_EXPONENT "PRINTF_EXPONENT_DIGITS=2" )
endif ( MINGW )


set ( PYINSTALLCHECK_ENVIRONMENT "AUTOTEST_PATH=${INSTALL_DESTDIR}${EXAMPLE_PATH}"
                                 "OPENTURNS_WRAPPER_PATH=${INSTALL_DESTDIR}${WRAPPER_PATH}"
                                 "OPENTURNS_CONFIG_PATH=${INSTALL_DESTDIR}${SYSCONFIG_PATH}/openturns"
                                 "OPENTURNS_NUMERICALSAMPLE_PATH=${INSTALL_DESTDIR}${SAMPLE_PATH}"
                                 "PATH=${NEW_PATH}"
                                 "LD_LIBRARY_PATH=${INSTALL_DESTDIR}${LIBRARY_PATH}"
                                 "PYTHONPATH=${INSTALL_DESTDIR}${PYTHON_MODULE_PATH}"
                                 ${ADAPT_CHECK_TESTS_EXPONENT}
    )

if ( MINGW )
  add_custom_target ( symlink_python_test_libot ALL
    ${CMAKE_COMMAND} -E create_symlink ${CMAKE_BINARY_DIR}/lib/src/libOT.dll ${CMAKE_CURRENT_BINARY_DIR}/libOT.dll
    COMMAND
    ${CMAKE_COMMAND} -E create_symlink ${CMAKE_BINARY_DIR}/lib/src/libOTbind.dll ${CMAKE_CURRENT_BINARY_DIR}/libOTbind.dll
    DEPENDS OT
    COMMENT "Add a symbolic link to libOT in python/test."
  )


  macro ( create_check_file FILE_NAME )
    file ( WRITE ${FILE_NAME} "#!/bin/sh\n\n" )
    file ( APPEND ${FILE_NAME} "#set -e\n\n" )
    #file ( APPEND ${FILE_NAME} "#${CHECK_ENVIRONMENT}\n\n" )
    file ( APPEND ${FILE_NAME} "export PRINTF_EXPONENT_DIGITS=2\n" )
    file ( APPEND ${FILE_NAME} "export AUTOTEST_PATH=$PWD\n" )
    file ( APPEND ${FILE_NAME} "export OPENTURNS_CONFIG_PATH=$PWD/..\n" )
    file ( APPEND ${FILE_NAME} "#export OPENTURNS_LOG_SEVERITY=ALL\n" )
    file ( APPEND ${FILE_NAME} "DEBUG_FILE=stdouterr.dbg \n" )
    file ( APPEND ${FILE_NAME} "export OPENTURNS_HOME=$PWD/..\n" )
    file ( APPEND ${FILE_NAME} "PATH=$PWD:$PWD/..:$PATH\n" )
    file ( APPEND ${FILE_NAME} "PYTHON_EXE=../../../../python.exe\n\n" )
    file ( APPEND ${FILE_NAME} "FAILED_TEST=\"\"\n" )
    file ( APPEND ${FILE_NAME} "function check_error {\nif [ \"$?\" != \"0\" ]; then\n  FAILED_TEST=\"$FAILED_TEST $1\"\n  echo \"std_err output:\"\n  cat $DEBUG_FILE\n  echo \"Test #$1: $2 failed!\"\n  echo\nfi\n}\n\n" )
  endmacro ( create_check_file FILE_NAME )

  macro ( add_check_file FILE_NAME TESTNAME EXTERNAL_COMMAND CUR_CHECK )
    file ( APPEND ${FILE_NAME} "echo \"Test #${CUR_CHECK}/$NB_CHECK: ${TESTNAME}\"\n" )
    file ( APPEND ${FILE_NAME} "${EXTERNAL_COMMAND}\n" )
    file ( APPEND ${FILE_NAME} "check_error ${CUR_CHECK} ${TESTNAME}\n\n" )
  endmacro ( add_check_file FILE_NAME TESTNAME EXTERNAL_COMMAND CUR_CHECK )

  macro ( finish_check_file FILE_NAME NB_CHECK )
    file ( APPEND ${FILE_NAME} "}\n\n" )
    file ( APPEND ${FILE_NAME} "NB_CHECK=${NB_CHECK}\n" )
    file ( APPEND ${FILE_NAME} "start_tests\necho\n" )
    file ( APPEND ${FILE_NAME} "RC=0\nif [ \"$FAILED_TEST\" = \"\" ]; then\n  echo \"Every check tests are OK.\"\n" )
    file ( APPEND ${FILE_NAME} "else\n  echo \"Tests$FAILED_TEST failed!\"\n  RC=1\nfi\n" )
    file ( APPEND ${FILE_NAME} "exit $RC\n" )
  endmacro ( finish_check_file FILE_NAME )


  set ( EXTERNAL_PYTHONINSTALLCHECK_FILE "${CMAKE_CURRENT_BINARY_DIR}/start_pythoninstallcheck_test.sh" )
  create_check_file ( ${EXTERNAL_PYTHONINSTALLCHECK_FILE} )
  #file ( APPEND ${EXTERNAL_PYTHONINSTALLCHECK_FILE} "#${INSTALLCHECK_ENVIRONMENT}\n\n" )
  file ( APPEND ${EXTERNAL_PYTHONINSTALLCHECK_FILE} "export OPENTURNS_NUMERICALSAMPLE_PATH=$PWD\n" )
  file ( APPEND ${EXTERNAL_PYTHONINSTALLCHECK_FILE} "export OPENTURNS_WRAPPER_PATH=$PWD/wrappers\n" )
  #file ( APPEND ${EXTERNAL_PYTHONINSTALLCHECK_FILE} "export abs_srcdir=$PWD\n" )
  file ( APPEND ${EXTERNAL_PYTHONINSTALLCHECK_FILE} "start_tests() {\n" )
  set ( CUR_PYTHONINSTALLCHECK 0 )

endif ( MINGW )

# This macro compiles on the fly and run the corresponding post-installation test
# It can have two optional arguments PRE and POST followed by some shell command to be run
# before or after the test if needed
# PRE and POST options cn be repeated many times: the arguments will be run one after the other
# in the order of appearence. If one fails, the whole command fails.
set ( PYINSTALLCHECK_TO_BE_RUN )
macro ( ot_pyinstallcheck_test TESTNAME )
  set ( TESTNAME_TGT t_${TESTNAME} )
  set ( TESTNAME_SRC ${TESTNAME_TGT}.py )
  #add_executable ( ${TESTNAME_TGT} EXCLUDE_FROM_ALL ${TESTNAME_SRC} )
  #target_link_libraries ( ${TESTNAME_TGT} OT )
  get_source_file_property ( TESTNAME_LOC ${TESTNAME_SRC} LOCATION )

  set ( _CMD )
  set ( _PRE )
  set ( _POST )
  set ( _PARAMS )
  set ( _IGNOREOUT OFF )
  foreach ( _ARG ${ARGN} )
    string ( TOUPPER ${_ARG} ARG )
    if ( ${ARG} MATCHES PRE )
      set ( _CMD PRE )
    elseif ( ${ARG} MATCHES POST )
      set ( _CMD POST )
    elseif ( ${ARG} MATCHES PARAMS )
      set ( _CMD PARAMS )
    elseif ( ${ARG} MATCHES IGNOREOUT )
      set ( _IGNOREOUT ON )
    else ( ${ARG} MATCHES PRE )
      if ( ${_CMD} MATCHES PRE )
        set ( _PRE "${_PRE} ${_ARG} && " )
      elseif ( ${_CMD} MATCHES POST )
        set ( _POST "${_POST} && ${_ARG}" )
      elseif ( ${_CMD} MATCHES PARAMS )
        set ( _PARAMS "${_PARAMS} ${_ARG}" )
      endif  ( ${_CMD} MATCHES PRE )
      set ( _CMD )
    endif ( ${ARG} MATCHES PRE )
  endforeach ( _ARG )
  
  if (_IGNOREOUT)
    set ( COMMAND "( ${PYTHON_EXECUTABLE} ${TESTNAME_LOC} ${_PARAMS} > /dev/null )" )
    set ( EXTERNAL_COMMAND "$PYTHON_EXE ${TESTNAME_SRC} > $DEBUG_FILE 2>&1" )
  else (_IGNOREOUT)
    set ( TESTNAME_OUT ${TESTNAME_TGT}.expout )
    get_source_file_property ( OUTFILE_LOC ${TESTNAME_OUT} LOCATION )
    set ( COMMAND "( ${PYTHON_EXECUTABLE} ${TESTNAME_LOC} ${_PARAMS} ${REPLACE_EOL} > ${CMAKE_CURRENT_BINARY_DIR}/${TESTNAME_TGT}.out ) && diff -u ${OUTFILE_LOC} ${CMAKE_CURRENT_BINARY_DIR}/${TESTNAME_TGT}.out" )
    set ( EXTERNAL_COMMAND "( $PYTHON_EXE ${TESTNAME_SRC} 2>$DEBUG_FILE | tr -d '\\\\r' > ${TESTNAME_TGT}.out ) && diff -u ${TESTNAME_SRC}.expout ${TESTNAME_TGT}.out" )
  endif (_IGNOREOUT)

  if (_PRE OR _POST)
    set ( COMMAND "${_PRE}${COMMAND}${_POST}" )
    #not useful
    #set ( EXTERNAL_COMMAND "${_PRE}${EXTERNAL_COMMAND}${_POST}" )
  endif (_PRE OR _POST)

  add_test ( pyinstallcheck_${TESTNAME} "sh" "-c" "${COMMAND}" )

  if ( MINGW )
    install ( FILES ${TESTNAME_OUT}
              DESTINATION ${EXAMPLE_PATH} 
              RENAME ${TESTNAME_SRC}.expout
              PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
            )
          add_check_file ( ${EXTERNAL_PYTHONINSTALLCHECK_FILE} ${TESTNAME_SRC} ${EXTERNAL_COMMAND} ${CUR_PYTHONINSTALLCHECK} )
    math( EXPR CUR_PYTHONINSTALLCHECK "${CUR_PYTHONINSTALLCHECK} + 1" )
  endif ( MINGW )


  set_tests_properties ( pyinstallcheck_${TESTNAME} PROPERTIES ENVIRONMENT "${PYINSTALLCHECK_ENVIRONMENT}" )
  list ( APPEND PYINSTALLCHECK_TO_BE_RUN ${TESTNAME_SRC} )
  install ( FILES ${TESTNAME_SRC}
            DESTINATION ${EXAMPLE_PATH} 
            PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
          )
endmacro ( ot_pyinstallcheck_test )


## Python post-installation tests

# Common
ot_pyinstallcheck_test ( ComparisonOperator_std )
if ( LIBXML2_FOUND )
ot_pyinstallcheck_test ( Study_saveload)
endif ()
ot_pyinstallcheck_test ( Path_std IGNOREOUT )
ot_pyinstallcheck_test ( ResourceMap_std IGNOREOUT )
ot_pyinstallcheck_test ( Catalog_std IGNOREOUT )

# Type
ot_pyinstallcheck_test ( Collection_std )
ot_pyinstallcheck_test ( Description_std )
ot_pyinstallcheck_test ( Indices_std )
ot_pyinstallcheck_test ( NumericalPoint_std )
ot_pyinstallcheck_test ( NumericalPoint_description )
ot_pyinstallcheck_test ( Matrix_std )
ot_pyinstallcheck_test ( SymmetricMatrixLapack_std )
ot_pyinstallcheck_test ( SquareMatrixLapack_std )
ot_pyinstallcheck_test ( SquareMatrix_std )
ot_pyinstallcheck_test ( SymmetricMatrix_std )
ot_pyinstallcheck_test ( Tensor_std )
ot_pyinstallcheck_test ( SymmetricTensor_std )
ot_pyinstallcheck_test ( MatrixSolveLinearSystem_std )
ot_pyinstallcheck_test ( MatrixSingularValues_std )
ot_pyinstallcheck_test ( IdentityMatrix_std )
ot_pyinstallcheck_test ( NumericalPoint_python )
ot_pyinstallcheck_test ( RegularGrid_std )
ot_pyinstallcheck_test ( ComplexMatrix_std )
ot_pyinstallcheck_test ( SquareComplexMatrix_std )
ot_pyinstallcheck_test ( HermitianMatrix_std )
ot_pyinstallcheck_test ( TriangularComplexMatrix_std )

# Func
ot_pyinstallcheck_test ( WrapperFile_std )
ot_pyinstallcheck_test ( WrapperFile_generic )
ot_pyinstallcheck_test ( NumericalMathFunction_std )
ot_pyinstallcheck_test ( NumericalMathFunction_grad )
ot_pyinstallcheck_test ( NumericalMathFunction_hess )
ot_pyinstallcheck_test ( NumericalMathFunction_sample )
ot_pyinstallcheck_test ( NumericalMathFunction_timeseries )
ot_pyinstallcheck_test ( NumericalMathFunction_composition )
ot_pyinstallcheck_test ( NumericalMathFunction_exec )
ot_pyinstallcheck_test ( NumericalMathFunction_exec_external )
ot_pyinstallcheck_test ( NumericalMathFunction_exec_sample )
ot_pyinstallcheck_test ( NumericalMathFunction_exec_threads )
ot_pyinstallcheck_test ( NumericalMathFunction_minimal )
ot_pyinstallcheck_test ( NumericalMathFunction_python )
ot_pyinstallcheck_test ( NumericalMathFunction_python_saveload )
ot_pyinstallcheck_test ( NumericalMathFunction_wrapper_error )
ot_pyinstallcheck_test ( NumericalMathFunction_data )
ot_pyinstallcheck_test ( NumericalMathFunction_product )
if ( R_base_FOUND )
ot_pyinstallcheck_test ( NumericalMathFunction_draw )
endif ()
ot_pyinstallcheck_test ( NumericalMathFunction_history )
ot_pyinstallcheck_test ( AggregatedNumericalMathEvaluationImplementation_std )
ot_pyinstallcheck_test ( AnalyticalNumericalMathEvaluationImplementation_std )
ot_pyinstallcheck_test ( AnalyticalNumericalMathGradientImplementation_std )
ot_pyinstallcheck_test ( AnalyticalNumericalMathHessianImplementation_std )
ot_pyinstallcheck_test ( LinearNumericalMathFunction_std )
ot_pyinstallcheck_test ( LinearNumericalMathEvaluationImplementation_std )
ot_pyinstallcheck_test ( LinearCombinationEvaluationImplementation_std )
ot_pyinstallcheck_test ( LinearCombinationGradientImplementation_std )
ot_pyinstallcheck_test ( LinearCombinationHessianImplementation_std )
ot_pyinstallcheck_test ( QuadraticNumericalMathEvaluationImplementation_std )
ot_pyinstallcheck_test ( LinearNumericalMathGradientImplementation_std )
ot_pyinstallcheck_test ( NumericalMathFunction_analytical )
ot_pyinstallcheck_test ( NumericalMathFunction_dual_linear_combination )
ot_pyinstallcheck_test ( NumericalMathFunction_indicator )
ot_pyinstallcheck_test ( NumericalMathFunction_linear_combination )
ot_pyinstallcheck_test ( ConstantNumericalMathGradientImplementation_std )
ot_pyinstallcheck_test ( ConstantNumericalMathHessianImplementation_std )
ot_pyinstallcheck_test ( RestrictedEvaluationImplementation_std )
ot_pyinstallcheck_test ( RestrictedGradientImplementation_std )
ot_pyinstallcheck_test ( RestrictedHessianImplementation_std )
ot_pyinstallcheck_test ( ConstantNumericalMathHessianImplementation_std )
ot_pyinstallcheck_test ( ConstantNumericalMathHessianImplementation_std )
ot_pyinstallcheck_test ( MergeRandomAndConstantInput )
ot_pyinstallcheck_test ( Basis_std )
ot_pyinstallcheck_test ( BasisSequence_std )
ot_pyinstallcheck_test ( LAR_std )
ot_pyinstallcheck_test ( DynamicalFunction_std )
ot_pyinstallcheck_test ( SpatialFunction_std )
ot_pyinstallcheck_test ( TemporalFunction_std )
ot_pyinstallcheck_test ( BoxCoxEvaluationImplementation_std )
ot_pyinstallcheck_test ( BoxCoxTransform_std )
ot_pyinstallcheck_test ( InverseBoxCoxEvaluationImplementation_std )
ot_pyinstallcheck_test ( InverseBoxCoxTransform_std )
ot_pyinstallcheck_test ( TrendEvaluationImplementation_std )
ot_pyinstallcheck_test ( TrendTransform_std )
ot_pyinstallcheck_test ( InverseTrendEvaluationImplementation_std )
ot_pyinstallcheck_test ( InverseTrendTransform_std )
ot_pyinstallcheck_test ( ExpertMixture_std )
ot_pyinstallcheck_test ( UniVariatePolynomial_std )

# Stat
ot_pyinstallcheck_test ( NumericalSample_std )
ot_pyinstallcheck_test ( NumericalSample_computation )
ot_pyinstallcheck_test ( NumericalSample_slice )
ot_pyinstallcheck_test ( NumericalSample_split )
ot_pyinstallcheck_test ( NumericalSample_large )
ot_pyinstallcheck_test ( NumericalSample_bug14 )
ot_pyinstallcheck_test ( NumericalSample_csv )
ot_pyinstallcheck_test ( HistoryStrategy_std )
ot_pyinstallcheck_test ( CovarianceMatrixLapack_std )
ot_pyinstallcheck_test ( CorrelationMatrix_std )
if ( R_rot_FOUND )
ot_pyinstallcheck_test ( LinearModel_std )
ot_pyinstallcheck_test ( LinearModelFactory_std )
endif ( R_rot_FOUND )
ot_pyinstallcheck_test ( ProcessSample_std )
ot_pyinstallcheck_test ( RandomGenerator_std )
ot_pyinstallcheck_test ( SobolSequence_std )
ot_pyinstallcheck_test ( FaureSequence_std )
ot_pyinstallcheck_test ( HaltonSequence_std )
ot_pyinstallcheck_test ( HaselgroveSequence_std )
ot_pyinstallcheck_test ( ReverseHaltonSequence_std )
ot_pyinstallcheck_test ( TestResult_std )
ot_pyinstallcheck_test ( ConfidenceInterval_std )
ot_pyinstallcheck_test ( SensitivityAnalysis_std )
ot_pyinstallcheck_test ( CorrelationAnalysis_std )
ot_pyinstallcheck_test ( TimeSeries_std )
ot_pyinstallcheck_test ( ExponentialCauchy_std )
ot_pyinstallcheck_test ( FilteringWindows_std )
ot_pyinstallcheck_test ( WelchFactory_std )
ot_pyinstallcheck_test ( ExponentialModel_std )
ot_pyinstallcheck_test ( CauchyModel_std )
ot_pyinstallcheck_test ( SecondOrderModel_std )
ot_pyinstallcheck_test ( UserDefinedSpectralModel_std )
ot_pyinstallcheck_test ( UserDefinedCovarianceModel_std )
ot_pyinstallcheck_test ( NonStationaryCovarianceModelFactory_std )
ot_pyinstallcheck_test ( UserDefinedStationaryCovarianceModel_std )
ot_pyinstallcheck_test ( StationaryCovarianceModelFactory_std )

# Geom
ot_pyinstallcheck_test ( Interval_std )
ot_pyinstallcheck_test ( Domain_std )
if ( R_base_FOUND )
ot_pyinstallcheck_test ( Mesh_std )
endif ()

## Graph
if ( R_base_FOUND )
ot_pyinstallcheck_test ( BarPlot_std )
ot_pyinstallcheck_test ( Cloud_std )
ot_pyinstallcheck_test ( Contour_std )
ot_pyinstallcheck_test ( Curve_std )
ot_pyinstallcheck_test ( Drawable_std )
ot_pyinstallcheck_test ( Graph_export )
ot_pyinstallcheck_test ( Graph_mix )
ot_pyinstallcheck_test ( Pairs_std )
ot_pyinstallcheck_test ( Pie_std )
ot_pyinstallcheck_test ( Staircase_std )
endif ()

## Diff
ot_pyinstallcheck_test ( FiniteDifferenceStep_std )
ot_pyinstallcheck_test ( CenteredFiniteDifferenceGradient_std )
ot_pyinstallcheck_test ( CenteredFiniteDifferenceHessian_std )
ot_pyinstallcheck_test ( NonCenteredFiniteDifferenceGradient_std )

## Optim
ot_pyinstallcheck_test ( AbdoRackwitz_std )
ot_pyinstallcheck_test ( AbdoRackwitz_linear )
ot_pyinstallcheck_test ( AbdoRackwitz_nonlinear )
ot_pyinstallcheck_test ( NearestPointChecker_std )
ot_pyinstallcheck_test ( Cobyla_std )
ot_pyinstallcheck_test ( Cobyla_linear )
ot_pyinstallcheck_test ( Cobyla_nonlinear )
ot_pyinstallcheck_test ( SQP_std )
ot_pyinstallcheck_test ( SQP_linear )
ot_pyinstallcheck_test ( SQP_nonlinear )
ot_pyinstallcheck_test ( TNC_std )
ot_pyinstallcheck_test ( TNC_linear )
ot_pyinstallcheck_test ( TNC_nonlinear )
ot_pyinstallcheck_test ( BoundConstrainedAlgorithmImplementationResult_std )

## Solver
ot_pyinstallcheck_test ( Brent_std )
ot_pyinstallcheck_test ( Bisection_std )
ot_pyinstallcheck_test ( Secant_std )

# Algo
ot_pyinstallcheck_test ( PenalizedLeastSquaresAlgorithm_std )
ot_pyinstallcheck_test ( KFold_std )
ot_pyinstallcheck_test ( CorrectedLeaveOneOut_std )
ot_pyinstallcheck_test ( LeastSquaresMetaModelSelectionFactory_std )
ot_pyinstallcheck_test ( KissFFT_std )

## Model
ot_pyinstallcheck_test ( Event_std )
ot_pyinstallcheck_test ( StandardEvent_std )
ot_pyinstallcheck_test ( RandomVector_function )
ot_pyinstallcheck_test ( Event_domain )
ot_pyinstallcheck_test ( RandomVector_constant )
ot_pyinstallcheck_test ( RandomVector_distribution )
ot_pyinstallcheck_test ( RandomVector_conditional )
ot_pyinstallcheck_test ( RandomVector_python )

## Process
ot_pyinstallcheck_test ( WhiteNoise_std )
ot_pyinstallcheck_test ( RandomWalk_std )
ot_pyinstallcheck_test ( ARMAState_std )
ot_pyinstallcheck_test ( ARMACoefficients_std )
ot_pyinstallcheck_test ( ARMA_std )
ot_pyinstallcheck_test ( TemporalNormalProcess_std )
ot_pyinstallcheck_test ( SpectralNormalProcess_std )
ot_pyinstallcheck_test ( CompositeProcess_std )
#ot_pyinstallcheck_test ( ARMALikelihoodFactory_std )
ot_pyinstallcheck_test ( FunctionalBasisProcess_std )
ot_pyinstallcheck_test ( WhittleFactory_std )

## Transformation
ot_pyinstallcheck_test ( MarginalTransformationEvaluation_std )
ot_pyinstallcheck_test ( MarginalTransformationGradient_std )
ot_pyinstallcheck_test ( MarginalTransformationHessian_std )
ot_pyinstallcheck_test ( InverseNatafIndependentCopulaEvaluation_std )
ot_pyinstallcheck_test ( InverseNatafIndependentCopulaGradient_std )
ot_pyinstallcheck_test ( InverseNatafIndependentCopulaHessian_std )
ot_pyinstallcheck_test ( NatafIndependentCopulaEvaluation_std )
ot_pyinstallcheck_test ( NatafIndependentCopulaGradient_std )
ot_pyinstallcheck_test ( NatafIndependentCopulaHessian_std )
ot_pyinstallcheck_test ( IsoProbabilisticTransformation_EllipticalDistribution )
ot_pyinstallcheck_test ( IsoProbabilisticTransformation_EllipticalCopula )
ot_pyinstallcheck_test ( IsoProbabilisticTransformation_IndependentCopula )
ot_pyinstallcheck_test ( BoxCoxFactory_std )
ot_pyinstallcheck_test ( TrendFactory_std )

## Distribution
ot_pyinstallcheck_test ( AliMikhailHaqCopula_std )
ot_pyinstallcheck_test ( AliMikhailHaqCopulaFactory_std )
ot_pyinstallcheck_test ( Arcsine_std )
ot_pyinstallcheck_test ( ArcsineFactory_std )
ot_pyinstallcheck_test ( Bernoulli_std )
ot_pyinstallcheck_test ( BernoulliFactory_std )
ot_pyinstallcheck_test ( Beta_std )
ot_pyinstallcheck_test ( BetaFactory_std )
ot_pyinstallcheck_test ( Binomial_std )
ot_pyinstallcheck_test ( BinomialFactory_std )
ot_pyinstallcheck_test ( Burr_std )
ot_pyinstallcheck_test ( BurrFactory_std )
ot_pyinstallcheck_test ( Chi_std )
ot_pyinstallcheck_test ( ChiFactory_std )
ot_pyinstallcheck_test ( ChiSquare_std )
ot_pyinstallcheck_test ( ChiSquareFactory_std )
ot_pyinstallcheck_test ( ClaytonCopula_std )
ot_pyinstallcheck_test ( ClaytonCopulaFactory_std )
ot_pyinstallcheck_test ( ComposedCopula_std )
ot_pyinstallcheck_test ( ComposedDistribution_std )
ot_pyinstallcheck_test ( ComposedDistribution_large )
ot_pyinstallcheck_test ( ConditionalDistribution_std )
ot_pyinstallcheck_test ( Dirac_std )
ot_pyinstallcheck_test ( DiracFactory_std )
ot_pyinstallcheck_test ( Dirichlet_std )
ot_pyinstallcheck_test ( DirichletFactory_std )
ot_pyinstallcheck_test ( Epanechnikov_std )
ot_pyinstallcheck_test ( Exponential_std )
ot_pyinstallcheck_test ( ExponentialFactory_std )
ot_pyinstallcheck_test ( FarlieGumbelMorgensternCopula_std )
ot_pyinstallcheck_test ( FarlieGumbelMorgensternCopulaFactory_std )
ot_pyinstallcheck_test ( FrankCopula_std )
ot_pyinstallcheck_test ( FrankCopulaFactory_std )
ot_pyinstallcheck_test ( FisherSnedecor_std )
ot_pyinstallcheck_test ( FisherSnedecorFactory_std )
ot_pyinstallcheck_test ( Gamma_std )
ot_pyinstallcheck_test ( GammaFactory_std )
ot_pyinstallcheck_test ( Geometric_std )
ot_pyinstallcheck_test ( GeometricFactory_std )
ot_pyinstallcheck_test ( Gumbel_std )
ot_pyinstallcheck_test ( GumbelFactory_std )
ot_pyinstallcheck_test ( GumbelCopula_std )
ot_pyinstallcheck_test ( GumbelCopulaFactory_std )
ot_pyinstallcheck_test ( Histogram_std )
ot_pyinstallcheck_test ( HistogramFactory_std )
ot_pyinstallcheck_test ( IndependentCopula_std )
ot_pyinstallcheck_test ( InverseNormal_std )
ot_pyinstallcheck_test ( InverseNormalFactory_std )
ot_pyinstallcheck_test ( KernelMixture_std )
ot_pyinstallcheck_test ( KernelSmoothing_std )
ot_pyinstallcheck_test ( Laplace_std )
ot_pyinstallcheck_test ( LaplaceFactory_std )
ot_pyinstallcheck_test ( Logistic_std )
ot_pyinstallcheck_test ( LogisticFactory_std )
ot_pyinstallcheck_test ( LogNormal_std )
ot_pyinstallcheck_test ( LogNormalFactory_std )
ot_pyinstallcheck_test ( LogUniform_std )
ot_pyinstallcheck_test ( LogUniformFactory_std )
ot_pyinstallcheck_test ( MinCopula_std )
ot_pyinstallcheck_test ( Mixture_std )
ot_pyinstallcheck_test ( Multinomial_std )
ot_pyinstallcheck_test ( MultinomialFactory_std )
ot_pyinstallcheck_test ( NegativeBinomial_std )
ot_pyinstallcheck_test ( NegativeBinomialFactory_std )
ot_pyinstallcheck_test ( NonCentralChiSquare_std )
ot_pyinstallcheck_test ( NonCentralStudent_std )
ot_pyinstallcheck_test ( Normal_std )
ot_pyinstallcheck_test ( NormalFactory_std )
ot_pyinstallcheck_test ( Normal_large )
ot_pyinstallcheck_test ( NormalCopula_std )
ot_pyinstallcheck_test ( NormalCopulaFactory_std )
ot_pyinstallcheck_test ( Normal_wrongarg )
ot_pyinstallcheck_test ( Poisson_std )
ot_pyinstallcheck_test ( PoissonFactory_std )
ot_pyinstallcheck_test ( RandomMixture_std )
ot_pyinstallcheck_test ( Rayleigh_std )
ot_pyinstallcheck_test ( RayleighFactory_std )
ot_pyinstallcheck_test ( Rice_std )
ot_pyinstallcheck_test ( RiceFactory_std )
ot_pyinstallcheck_test ( SklarCopula_std )
ot_pyinstallcheck_test ( Student_std )
ot_pyinstallcheck_test ( StudentFactory_std )
ot_pyinstallcheck_test ( Trapezoidal_std )
ot_pyinstallcheck_test ( TrapezoidalFactory_std )
ot_pyinstallcheck_test ( Triangular_std )
ot_pyinstallcheck_test ( TriangularFactory_std )
ot_pyinstallcheck_test ( TruncatedDistribution_std )
ot_pyinstallcheck_test ( TruncatedNormal_std )
if ( R_rot_FOUND )
ot_pyinstallcheck_test ( TruncatedNormalFactory_std )
endif ( R_rot_FOUND )
ot_pyinstallcheck_test ( Uniform_std )
ot_pyinstallcheck_test ( UniformFactory_std )
ot_pyinstallcheck_test ( UserDefined_std )
ot_pyinstallcheck_test ( UserDefinedFactory_std )
ot_pyinstallcheck_test ( Weibull_std )
ot_pyinstallcheck_test ( WeibullFactory_std )
ot_pyinstallcheck_test ( ZipfMandelbrot_std )
ot_pyinstallcheck_test ( DistFunc_beta )
ot_pyinstallcheck_test ( DistFunc_gamma )
ot_pyinstallcheck_test ( DistFunc_kolmogorov )
ot_pyinstallcheck_test ( DistFunc_noncentralchisquare )
ot_pyinstallcheck_test ( DistFunc_noncentralstudent )
ot_pyinstallcheck_test ( DistFunc_normal )
ot_pyinstallcheck_test ( DistFunc_poisson )
ot_pyinstallcheck_test ( DistFunc_student )
if ( R_base_FOUND )
ot_pyinstallcheck_test ( Distributions_draw )
endif ()
ot_pyinstallcheck_test ( Distribution_python )

## Bayesian
ot_pyinstallcheck_test ( CalibrationStrategy_std )
ot_pyinstallcheck_test ( RandomWalkMetropolisHastings_std )
ot_pyinstallcheck_test ( RandomWalkMetropolisHastings_regression IGNOREOUT )
ot_pyinstallcheck_test ( PosteriorRandomVector_std )

## Classification
ot_pyinstallcheck_test ( MixtureClassifier_std )

## Experiments
ot_pyinstallcheck_test ( Axial_std )
ot_pyinstallcheck_test ( Box_std )
ot_pyinstallcheck_test ( Composite_std )
ot_pyinstallcheck_test ( Factorial_std )
ot_pyinstallcheck_test ( BootstrapExperiment_std )
ot_pyinstallcheck_test ( FixedExperiment_std )
ot_pyinstallcheck_test ( GaussProductExperiment_std )
ot_pyinstallcheck_test ( ImportanceSamplingExperiment_std )
ot_pyinstallcheck_test ( LHSExperiment_std )
ot_pyinstallcheck_test ( LowDiscrepancyExperiment_std )
ot_pyinstallcheck_test ( MonteCarloExperiment_std )
ot_pyinstallcheck_test ( MinMax_computation )

## OrthogonalBasis
ot_pyinstallcheck_test ( LinearEnumerateFunction_std )
ot_pyinstallcheck_test ( HyperbolicAnisotropicEnumerateFunction_std )
ot_pyinstallcheck_test ( CharlierFactory_std )
ot_pyinstallcheck_test ( HermiteFactory_std )
ot_pyinstallcheck_test ( JacobiFactory_std )
ot_pyinstallcheck_test ( KrawtchoukFactory_std )
ot_pyinstallcheck_test ( LaguerreFactory_std )
ot_pyinstallcheck_test ( LegendreFactory_std )
ot_pyinstallcheck_test ( MeixnerFactory_std )
ot_pyinstallcheck_test ( OrthonormalizationAlgorithm_std )
ot_pyinstallcheck_test ( ChebychevAlgorithm_std )
ot_pyinstallcheck_test ( GramSchmidtAlgorithm_std )
ot_pyinstallcheck_test ( StandardDistributionPolynomialFactory_std )
ot_pyinstallcheck_test ( OrthogonalBasis_std )

## QuadraticCumul
ot_pyinstallcheck_test ( QuadraticCumul_std )
ot_pyinstallcheck_test ( QuadraticCumul_importanceFactors )
ot_pyinstallcheck_test ( QuadraticCumul_run )

## Analytical
ot_pyinstallcheck_test ( Analytical_std )
ot_pyinstallcheck_test ( AnalyticalResult_std )
ot_pyinstallcheck_test ( FORM_std )
if ( R_base_FOUND )
ot_pyinstallcheck_test ( FORM_draw )
endif ()
ot_pyinstallcheck_test ( FORM_sensitivity )
ot_pyinstallcheck_test ( SORM_std )
ot_pyinstallcheck_test ( StrongMaximumTest_std )
ot_pyinstallcheck_test ( StrongMaximumTest_other )
ot_pyinstallcheck_test ( StrongMaximumTest_tabulatedValues )

## MetaModel
ot_pyinstallcheck_test ( LinearTaylor_std )
ot_pyinstallcheck_test ( LinearLeastSquares_std )
ot_pyinstallcheck_test ( QuadraticLeastSquares_std )
ot_pyinstallcheck_test ( QuadraticTaylor_std )
ot_pyinstallcheck_test ( FunctionalChaos_ishigami )
ot_pyinstallcheck_test ( FunctionalChaos_ishigami_sparse )
ot_pyinstallcheck_test ( FunctionalChaos_ishigami_database )
ot_pyinstallcheck_test ( FunctionalChaos_gsobol )
ot_pyinstallcheck_test ( FunctionalChaos_gsobol_sparse )
ot_pyinstallcheck_test ( FunctionalChaos_nd )

## Simulation
ot_pyinstallcheck_test ( Wilks_std )
if ( R_base_FOUND )
ot_pyinstallcheck_test ( SimulationSensitivityAnalysis_std )
endif ()
ot_pyinstallcheck_test ( MonteCarlo_std )
if ( R_base_FOUND )
ot_pyinstallcheck_test ( MonteCarlo_draw )
endif ()
ot_pyinstallcheck_test ( MonteCarlo_threads )
ot_pyinstallcheck_test ( MonteCarlo_sensitivity )

ot_pyinstallcheck_test ( QuasiMonteCarlo_std )
ot_pyinstallcheck_test ( RandomizedQuasiMonteCarlo_std )
ot_pyinstallcheck_test ( LHS_std )
ot_pyinstallcheck_test ( RandomizedLHS_std )
ot_pyinstallcheck_test ( ImportanceSampling_std )
ot_pyinstallcheck_test ( DirectionalSampling_std )
ot_pyinstallcheck_test ( PostAnalyticalControlledImportanceSampling_std )

## StatTests
if ( R_rot_FOUND )
ot_pyinstallcheck_test ( VisualTest_std )
ot_pyinstallcheck_test ( FittingTest_std )
ot_pyinstallcheck_test ( HypothesisTest_std )
# ot_pyinstallcheck_test ( HypothesisTest_correlation )
ot_pyinstallcheck_test ( NormalityTest_std )
ot_pyinstallcheck_test ( LinearModelTest_std )
endif ()
#ot_pyinstallcheck_test ( DickeyFullerTest_std )

## Waarts
ot_pyinstallcheck_test ( Waarts_25_quadratic_terms )
# ot_pyinstallcheck_test ( Waarts_concave )
# ot_pyinstallcheck_test ( Waarts_convex )
# ot_pyinstallcheck_test ( Waarts_discontinuous_lsf )
# ot_pyinstallcheck_test ( Waarts_noisy_lsf )
# ot_pyinstallcheck_test ( Waarts_RS2 )
ot_pyinstallcheck_test ( Waarts_system_series )
ot_pyinstallcheck_test ( Waarts_saddle )

## Misc
if ( NUMPY_FOUND )
  ot_pyinstallcheck_test ( NumpyTypeConversion_std )
endif ()
if ( SCIPY_FOUND )
  ot_pyinstallcheck_test ( SciPyDistribution_std )
endif ()
if ( MATPLOTLIB_FOUND )
  ot_pyinstallcheck_test ( Viewer IGNOREOUT )
endif ()
ot_pyinstallcheck_test ( QuickTest IGNOREOUT )


## Distributed python wrapper
ot_pyinstallcheck_test ( coupling_tools )
ot_pyinstallcheck_test ( wrapper_data )
ot_pyinstallcheck_test ( distributed_python_wrapper_std PARAMS "${CMAKE_CURRENT_SOURCE_DIR}" )
ot_pyinstallcheck_test ( distributed_python_wrapper_template PARAMS "${CMAKE_CURRENT_SOURCE_DIR}" )
if ( ENABLE_DISTRIBUTED_TEST )
  ot_pyinstallcheck_test ( remote_communicator )
  ot_pyinstallcheck_test ( distributed_python_wrapper_remote PARAMS "${CMAKE_CURRENT_SOURCE_DIR}" )
endif ( ENABLE_DISTRIBUTED_TEST )

add_custom_target ( pyinstallcheck COMMAND ${CMAKE_CTEST_COMMAND} -R "^pyinstallcheck_"
                    DEPENDS ${PYINSTALLCHECK_TO_BE_RUN}
                    COMMENT "Run Python post-installation tests" )


if ( MINGW )
  finish_check_file ( ${EXTERNAL_PYTHONINSTALLCHECK_FILE} ${CUR_PYTHONINSTALLCHECK} )

  install ( FILES ${EXTERNAL_PYTHONINSTALLCHECK_FILE} t_distributed_python_wrapper.py 
            dummy_func_wrapper.py dummy_program.py dummy_program_wrapper.py
            DESTINATION ${EXAMPLE_PATH} 
            PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
          )

endif ( MINGW )

