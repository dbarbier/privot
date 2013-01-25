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

set ( CHECK_ENVIRONMENT "OPENTURNS_CONFIG_PATH=${CMAKE_BINARY_DIR}/lib/etc"
                        ${ADAPT_CHECK_TESTS_EXPONENT} 
    )

set ( INSTALLCHECK_ENVIRONMENT "AUTOTEST_PATH=${INSTALL_DESTDIR}${EXAMPLE_PATH}"
                               "OPENTURNS_WRAPPER_PATH=${INSTALL_DESTDIR}${WRAPPER_PATH}"
                               "OPENTURNS_CONFIG_PATH=${INSTALL_DESTDIR}${SYSCONFIG_PATH}/openturns"
                               "OPENTURNS_NUMERICALSAMPLE_PATH=${INSTALL_DESTDIR}${SAMPLE_PATH}"
                               "PATH=${NEW_PATH}"
                               ${ADAPT_CHECK_TESTS_EXPONENT} 
    )

if ( MINGW )
  add_custom_target ( symlink_lib_test_libot
    ${CMAKE_COMMAND} -E create_symlink ${CMAKE_BINARY_DIR}/lib/src/libOT.dll ${CMAKE_CURRENT_BINARY_DIR}/libOT.dll
    COMMAND
    ${CMAKE_COMMAND} -E create_symlink ${CMAKE_BINARY_DIR}/lib/src/libOTbind.dll ${CMAKE_CURRENT_BINARY_DIR}/libOTbind.dll
    DEPENDS OT
    COMMENT "Add a symbolic link to libOT in lib/test."
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
    file ( APPEND ${FILE_NAME} "PATH=$PWD:$PWD/..:$PATH\n" )
    file ( APPEND ${FILE_NAME} "FAILED_TEST=\"\"\n\n" )
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

  # create a standalone sh file that can launch tests
  set ( EXTERNAL_CHECK_FILE "${CMAKE_CURRENT_BINARY_DIR}/start_check_test.sh" )
  create_check_file ( ${EXTERNAL_CHECK_FILE} )
  file ( APPEND ${EXTERNAL_CHECK_FILE} "\nstart_tests() {\n" )
  set ( CUR_CHECK 0 )
endif ( MINGW )


# This macro compiles on the fly and run the corresponding pre-installation test
# It can have two optional arguments PRE and POST followed by some shell command to be run
# before or after the test if needed
# PRE and POST options cn be repeated many times: the arguments will be run one after the other
# in the order of appearence. If one fails, the whole command fails.
set ( CHECK_TO_BE_RUN )
macro ( ot_check_test TESTNAME )
  set ( TESTNAME_TGT t_${TESTNAME} )
  set ( TESTNAME_SRC ${TESTNAME_TGT}.cxx )
  if ( MINGW )
    add_executable ( ${TESTNAME_TGT} ${TESTNAME_SRC} )
  else ( MINGW )
    add_executable ( ${TESTNAME_TGT} EXCLUDE_FROM_ALL ${TESTNAME_SRC} )
  endif ( MINGW )
  target_link_libraries ( ${TESTNAME_TGT} OT )
  if ( MINGW )
    set_target_properties ( ${TESTNAME_TGT} PROPERTIES
                                            LINK_FLAGS "-Wl,--enable-auto-import" )
    add_dependencies( ${TESTNAME_TGT} symlink_lib_test_libot ) 
  endif ( MINGW )
  set_target_properties ( ${TESTNAME_TGT} PROPERTIES
                                          INSTALL_RPATH "${CMAKE_BINARY_DIR}/lib/src;${CMAKE_INSTALL_RPATH}" )
  get_target_property ( TESTNAME_LOC ${TESTNAME_TGT} LOCATION )

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
    set ( COMMAND "( ${TESTNAME_LOC} ${_PARAMS} > /dev/null )" )
    set ( EXTERNAL_COMMAND "./${TESTNAME_TGT} ${_PARAMS} > $DEBUG_FILE 2>&1" )
  else (_IGNOREOUT)
    set ( TESTNAME_OUT ${TESTNAME_TGT}.expout )
    get_source_file_property ( OUTFILE_LOC ${TESTNAME_OUT} LOCATION )
    set ( COMMAND "( ${TESTNAME_LOC} ${_PARAMS} ${REPLACE_EOL} > ${CMAKE_CURRENT_BINARY_DIR}/${TESTNAME_TGT}.out ) && diff -u ${OUTFILE_LOC} ${CMAKE_CURRENT_BINARY_DIR}/${TESTNAME_TGT}.out" )
    set ( EXTERNAL_COMMAND "( ./${TESTNAME_TGT} ${_PARAMS} 2>$DEBUG_FILE | tr -d '\\\\r' > ${TESTNAME_TGT}.out ) && diff -u ${TESTNAME_OUT} ${TESTNAME_TGT}.out" )
  endif (_IGNOREOUT)

  if (_PRE OR _POST)
    set ( COMMAND "${_PRE}${COMMAND}${_POST}" )
    set ( EXTERNAL_COMMAND "${_PRE}${EXTERNAL_COMMAND}${_POST}" )
  endif (_PRE OR _POST)

  add_test ( cppcheck_${TESTNAME} "sh" "-c" "${COMMAND}" )
  set_tests_properties ( cppcheck_${TESTNAME} PROPERTIES ENVIRONMENT "${CHECK_ENVIRONMENT}" )
  list ( APPEND CHECK_TO_BE_RUN ${TESTNAME_TGT} )
  install ( FILES ${TESTNAME_SRC}
            DESTINATION ${EXAMPLE_PATH} 
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
          )

  if ( MINGW )
    install ( FILES ${TESTNAME_OUT}
              DESTINATION ${EXAMPLE_PATH} 
              PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
            )
    install ( TARGETS ${TESTNAME_TGT}
              DESTINATION ${EXAMPLE_PATH} 
            )

    add_check_file ( ${EXTERNAL_CHECK_FILE} ${TESTNAME_TGT} ${EXTERNAL_COMMAND} ${CUR_CHECK} )
    math( EXPR CUR_CHECK "${CUR_CHECK} + 1" )
  endif ( MINGW )

endmacro ( ot_check_test )


# create a standalone sh file that can launch tests
if ( MINGW )
  set ( EXTERNAL_INSTALLCHECK_FILE "${CMAKE_CURRENT_BINARY_DIR}/start_installcheck_test.sh" )
  create_check_file ( ${EXTERNAL_INSTALLCHECK_FILE} )
  #file ( APPEND ${EXTERNAL_INSTALLCHECK_FILE} "#${INSTALLCHECK_ENVIRONMENT}\n\n" )
  file ( APPEND ${EXTERNAL_INSTALLCHECK_FILE} "export OPENTURNS_NUMERICALSAMPLE_PATH=$PWD\n" )
  file ( APPEND ${EXTERNAL_INSTALLCHECK_FILE} "export OPENTURNS_WRAPPER_PATH=$PWD/wrappers\n" )
  #file ( APPEND ${EXTERNAL_INSTALLCHECK_FILE} "export abs_srcdir=$PWD\n" )
  file ( APPEND ${EXTERNAL_INSTALLCHECK_FILE} "start_tests() {\n" )
  set ( CUR_INSTALLCHECK 0 )
endif ( MINGW )


# This macro compiles on the fly and run the corresponding post-installation test
# It can have two optional arguments PRE and POST followed by some shell command to be run
# before or after the test if needed
# PRE and POST options cn be repeated many times: the arguments will be run one after the other
# in the order of appearence. If one fails, the whole command fails.
set ( INSTALLCHECK_TO_BE_RUN )
macro ( ot_installcheck_test TESTNAME )
  set ( TESTNAME_TGT t_${TESTNAME} )
  set ( TESTNAME_SRC ${TESTNAME_TGT}.cxx )
  if ( MINGW )
    add_executable ( ${TESTNAME_TGT} ${TESTNAME_SRC} )
  else ( MINGW )
    add_executable ( ${TESTNAME_TGT} EXCLUDE_FROM_ALL ${TESTNAME_SRC} )
  endif ( MINGW )
  target_link_libraries ( ${TESTNAME_TGT} OT )
  if ( MINGW )
    set_target_properties ( ${TESTNAME_TGT} PROPERTIES
                                            LINK_FLAGS "-Wl,--enable-auto-import" )
    add_dependencies( ${TESTNAME_TGT} makelinkot ) 
  endif ( MINGW )
  get_target_property ( TESTNAME_LOC ${TESTNAME_TGT} LOCATION )

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
    set ( COMMAND "( ${TESTNAME_LOC} ${_PARAMS} > /dev/null )" )
    set ( EXTERNAL_COMMAND "./${TESTNAME_TGT} ${_PARAMS} > $DEBUG_FILE 2>&1" )
  else (_IGNOREOUT)
    set ( TESTNAME_OUT ${TESTNAME_TGT}.expout )
    get_source_file_property ( OUTFILE_LOC ${TESTNAME_OUT} LOCATION )
    set ( COMMAND "( ${TESTNAME_LOC} ${_PARAMS} > ${CMAKE_CURRENT_BINARY_DIR}/${TESTNAME_TGT}.out ) && diff -u ${OUTFILE_LOC} ${CMAKE_CURRENT_BINARY_DIR}/${TESTNAME_TGT}.out" )
    set ( EXTERNAL_COMMAND "( ./${TESTNAME_TGT} ${_PARAMS} 2>$DEBUG_FILE | tr -d '\\\\r' > ${TESTNAME_TGT}.out ) && diff -u ${TESTNAME_OUT} ${TESTNAME_TGT}.out" )
  endif (_IGNOREOUT)

  if (_PRE OR _POST)
    set ( COMMAND "${_PRE}${COMMAND}${_POST}" )
    #not useful
    #set ( EXTERNAL_COMMAND "${_PRE}${EXTERNAL_COMMAND}${_POST}" )
  endif (_PRE OR _POST)

  add_test ( cppinstallcheck_${TESTNAME} "sh" "-c" "${COMMAND}" )
  set_tests_properties ( cppinstallcheck_${TESTNAME} PROPERTIES ENVIRONMENT "${INSTALLCHECK_ENVIRONMENT}" )
  list ( APPEND INSTALLCHECK_TO_BE_RUN ${TESTNAME_TGT} )
  install ( FILES ${TESTNAME_SRC}
            DESTINATION ${EXAMPLE_PATH}
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
          )

  if ( MINGW )
    install ( FILES ${TESTNAME_OUT}
              DESTINATION ${EXAMPLE_PATH} 
              PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
            )
    install ( TARGETS ${TESTNAME_TGT}
              DESTINATION ${EXAMPLE_PATH} 
            )

    add_check_file ( ${EXTERNAL_INSTALLCHECK_FILE} ${TESTNAME_TGT} ${EXTERNAL_COMMAND} ${CUR_INSTALLCHECK} )
    math( EXPR CUR_INSTALLCHECK "${CUR_INSTALLCHECK} + 1" )
  endif ( MINGW )

endmacro ( ot_installcheck_test )

include_directories ( ${INTERNAL_INCLUDE_DIRS} )

## Pre-installation tests

# Common
ot_check_test ( Object_std )
ot_check_test ( OStream_std )
ot_check_test ( PersistentObject_std )
ot_check_test ( Pointer_std )
ot_check_test ( Pointer_cast )
ot_check_test ( Exception_std )
ot_check_test ( Log_std )
ot_check_test ( IdFactory_std IGNOREOUT )
ot_check_test ( ComparisonOperator_std )
if ( LIBXML2_FOUND )
ot_check_test ( Study_saveload  )
endif ()
ot_check_test ( ResourceMap_std IGNOREOUT )
ot_check_test ( Catalog_std IGNOREOUT )
ot_check_test ( AtomicFunctions_std IGNOREOUT )

# Type
ot_check_test ( Collection_std )
ot_check_test ( Indices_std )
ot_check_test ( Description_std )
ot_check_test ( NumericalPoint_std )
ot_check_test ( NumericalPoint_description )
ot_check_test ( Matrix_std )
ot_check_test ( IdentityMatrix_std )
ot_check_test ( Tensor_std )
ot_check_test ( SymmetricTensor_std )
ot_check_test ( SquareMatrix_std )
ot_check_test ( SymmetricMatrix_std )
ot_check_test ( MatrixSolveLinearSystem_std )
ot_check_test ( MatrixSingularValues_std )
ot_check_test ( SquareMatrixLapack_std )
ot_check_test ( SymmetricMatrixLapack_std )
ot_check_test ( Cache_std )
ot_check_test ( RegularGrid_std )
ot_check_test ( ComplexMatrix_std )
ot_check_test ( SquareComplexMatrix_std )
ot_check_test ( HermitianMatrix_std )
ot_check_test ( TriangularComplexMatrix_std )

# Func
ot_check_test ( LibraryLoader_std )
ot_check_test ( LinearNumericalMathEvaluationImplementation_std )
ot_check_test ( ConstantNumericalMathGradientImplementation_std )
ot_check_test ( AggregatedNumericalMathEvaluationImplementation_std )
ot_check_test ( PiecewiseEvaluationImplementation_std )
ot_check_test ( AnalyticalNumericalMathEvaluationImplementation_std )
ot_check_test ( AnalyticalNumericalMathGradientImplementation_std )
ot_check_test ( AnalyticalNumericalMathHessianImplementation_std )
ot_check_test ( LinearNumericalMathFunction_std )
ot_check_test ( LinearCombinationEvaluationImplementation_std )
ot_check_test ( LinearCombinationGradientImplementation_std )
ot_check_test ( LinearCombinationHessianImplementation_std )
ot_check_test ( QuadraticNumericalMathEvaluationImplementation_std )
ot_check_test ( LinearNumericalMathGradientImplementation_std )
ot_check_test ( ConstantNumericalMathHessianImplementation_std )
ot_check_test ( RestrictedEvaluationImplementation_std )
ot_check_test ( RestrictedGradientImplementation_std )
ot_check_test ( RestrictedHessianImplementation_std )
ot_check_test ( MethodBoundNumericalMathEvaluationImplementation_std )
ot_check_test ( NumericalMathFunction_analytical )
ot_check_test ( NumericalMathFunction_indicator )
ot_check_test ( NumericalMathFunction_dual_linear_combination )
ot_check_test ( NumericalMathFunction_linear_combination )
ot_check_test ( NumericalMathFunction_product )
ot_check_test ( NumericalMathFunction_data )
ot_check_test ( NumericalMathFunction_history )
if ( R_base_FOUND )
ot_check_test ( NumericalMathFunction_draw )
endif ()
ot_check_test ( UniVariatePolynomial_std )
ot_check_test ( Basis_std )
ot_check_test ( BasisSequence_std )
ot_check_test ( LAR_std )
ot_check_test ( DynamicalFunction_std )
ot_check_test ( SpatialFunction_std )
ot_check_test ( TemporalFunction_std )
ot_check_test ( BoxCoxEvaluationImplementation_std )
ot_check_test ( BoxCoxTransform_std )
ot_check_test ( InverseBoxCoxEvaluationImplementation_std )
ot_check_test ( InverseBoxCoxTransform_std )
ot_check_test ( TrendEvaluationImplementation_std )
ot_check_test ( TrendTransform_std )
ot_check_test ( InverseTrendEvaluationImplementation_std )
ot_check_test ( InverseTrendTransform_std )
ot_check_test ( ExpertMixture_std)

# Optim
ot_check_test ( BoundConstrainedAlgorithmImplementationResult_std )

# Solver
ot_check_test ( Bisection_std )
ot_check_test ( Brent_std )
ot_check_test ( Secant_std )
ot_check_test ( RungeKutta_std )

# Stat
ot_check_test ( NumericalSample_std )
ot_check_test ( NumericalSample_split )
ot_check_test ( NumericalSample_large )
ot_check_test ( NumericalSample_computation )
ot_check_test ( HistoryStrategy_std )
ot_check_test ( CovarianceMatrixLapack_std )
ot_check_test ( CorrelationMatrix_std )
ot_check_test ( ConfidenceInterval_std )
ot_check_test ( TestResult_std )
if ( R_rot_FOUND )
ot_check_test ( LinearModelFactory_std )
ot_check_test ( LinearModel_std )
endif ( R_rot_FOUND )
ot_check_test ( SensitivityAnalysis_std )
ot_check_test ( ProcessSample_std )
ot_check_test ( RandomGenerator_std )
ot_check_test ( SobolSequence_std )
ot_check_test ( FaureSequence_std )
ot_check_test ( HaltonSequence_std )
ot_check_test ( HaselgroveSequence_std )
ot_check_test ( ReverseHaltonSequence_std )
ot_check_test ( CorrelationAnalysis_std )
ot_check_test ( TimeSeries_std )
ot_check_test ( ExponentialCauchy_std )
ot_check_test ( FilteringWindows_std )
ot_check_test ( WelchFactory_std )
ot_check_test ( ExponentialModel_std )
ot_check_test ( CauchyModel_std )
ot_check_test ( SecondOrderModel_std )
ot_check_test ( UserDefinedSpectralModel_std )
ot_check_test ( UserDefinedCovarianceModel_std )
ot_check_test ( NonStationaryCovarianceModelFactory_std )
ot_check_test ( UserDefinedStationaryCovarianceModel_std )
ot_check_test ( StationaryCovarianceModelFactory_std )

# Geom
ot_check_test ( Domain_std )
ot_check_test ( Interval_std )
if ( R_base_FOUND )
ot_check_test ( Mesh_std )
endif ()

# Graph
if ( R_base_FOUND )
ot_check_test ( BarPlot_std )
ot_check_test ( Cloud_std )
ot_check_test ( Contour_std )
ot_check_test ( Curve_std )
ot_check_test ( Drawable_std )
ot_check_test ( Graph_mix )
ot_check_test ( Graph_export )
ot_check_test ( Pairs_std )
ot_check_test ( Pie_std )
ot_check_test ( Staircase_std )
endif ()

# Algo
ot_check_test ( PenalizedLeastSquaresAlgorithm_std )
ot_check_test ( CorrectedLeaveOneOut_std )
ot_check_test ( KFold_std )
# ot_check_test ( Lasso_std )
# ot_check_test ( LassoFactory_std )
ot_check_test ( LeastSquaresMetaModelSelectionFactory_std )
ot_check_test ( KissFFT_std )

# Distribution
ot_check_test ( Arcsine_std )
ot_check_test ( ArcsineFactory_std )
ot_check_test ( Bernoulli_std )
ot_check_test ( BernoulliFactory_std )
ot_check_test ( Beta_std )
ot_check_test ( BetaFactory_std )
ot_check_test ( Binomial_std )
ot_check_test ( BinomialFactory_std )
ot_check_test ( Burr_std )
ot_check_test ( BurrFactory_std )
ot_check_test ( Chi_std )
ot_check_test ( ChiFactory_std )
ot_check_test ( ChiSquare_std )
ot_check_test ( ChiSquareFactory_std )
ot_check_test ( ComposedCopula_std )
ot_check_test ( ComposedDistribution_std )
ot_check_test ( ComposedDistribution_large )
ot_check_test ( ConditionalDistribution_std )
ot_check_test ( Dirac_std )
ot_check_test ( DiracFactory_std )
ot_check_test ( Dirichlet_std )
ot_check_test ( DirichletFactory_std )
ot_check_test ( Epanechnikov_std )
ot_check_test ( Exponential_std )
ot_check_test ( ExponentialFactory_std )
ot_check_test ( FisherSnedecor_std )
ot_check_test ( FisherSnedecorFactory_std )
ot_check_test ( Gamma_std )
ot_check_test ( GammaFactory_std )
ot_check_test ( Geometric_std )
ot_check_test ( GeometricFactory_std )
ot_check_test ( Gumbel_std )
ot_check_test ( GumbelFactory_std )
ot_check_test ( Histogram_std )
ot_check_test ( HistogramFactory_std )
ot_check_test ( InverseNormal_std )
ot_check_test ( InverseNormalFactory_std )
ot_check_test ( KSubset_std )
ot_check_test ( KernelMixture_std )
ot_check_test ( KernelSmoothing_std )
ot_check_test ( Laplace_std )
ot_check_test ( LaplaceFactory_std )
ot_check_test ( Logistic_std )
ot_check_test ( LogisticFactory_std )
ot_check_test ( LogNormal_std )
ot_check_test ( LogNormalFactory_std )
ot_check_test ( LogUniform_std )
ot_check_test ( LogUniformFactory_std )
ot_check_test ( Mixture_std )
ot_check_test ( Multinomial_std )
ot_check_test ( MultinomialFactory_std )
ot_check_test ( NegativeBinomial_std )
ot_check_test ( NegativeBinomialFactory_std )
ot_check_test ( NonCentralChiSquare_std )
ot_check_test ( NonCentralStudent_std )
ot_check_test ( Normal_std )
ot_check_test ( NormalFactory_std )
ot_check_test ( Normal_large )
ot_check_test ( Normal_wrongarg )
ot_check_test ( Poisson_std )
ot_check_test ( PoissonFactory_std )
#ot_check_test ( PosteriorDistribution_std )
ot_check_test ( RandomMixture_std )
ot_check_test ( Rayleigh_std )
ot_check_test ( RayleighFactory_std )
ot_check_test ( Rice_std )
ot_check_test ( RiceFactory_std )
ot_check_test ( SklarCopula_std )
ot_check_test ( Student_std )
ot_check_test ( StudentFactory_std )
ot_check_test ( Trapezoidal_std )
ot_check_test ( TrapezoidalFactory_std )
ot_check_test ( Triangular_std )
ot_check_test ( TriangularFactory_std )
ot_check_test ( TruncatedDistribution_std )
ot_check_test ( TruncatedNormal_std )
if ( R_rot_FOUND )
ot_check_test ( TruncatedNormalFactory_std )
endif ( R_rot_FOUND )
ot_check_test ( Uniform_std )
ot_check_test ( UniformFactory_std )
ot_check_test ( UserDefined_std )
ot_check_test ( UserDefinedFactory_std )
ot_check_test ( Weibull_std )
ot_check_test ( WeibullFactory_std )
ot_check_test ( ZipfMandelbrot_std )
ot_check_test ( IndependentCopula_std )
ot_check_test ( MinCopula_std )
ot_check_test ( NormalCopula_std )
ot_check_test ( NormalCopulaFactory_std )
ot_check_test ( FarlieGumbelMorgensternCopula_std )
ot_check_test ( FarlieGumbelMorgensternCopulaFactory_std )
ot_check_test ( AliMikhailHaqCopula_std )
ot_check_test ( AliMikhailHaqCopulaFactory_std )
ot_check_test ( FrankCopula_std )
ot_check_test ( FrankCopulaFactory_std )
ot_check_test ( ClaytonCopula_std )
ot_check_test ( ClaytonCopulaFactory_std )
ot_check_test ( GumbelCopula_std )
ot_check_test ( GumbelCopulaFactory_std )
ot_check_test ( DistFunc_beta )
ot_check_test ( DistFunc_gamma )
ot_check_test ( DistFunc_kolmogorov )
ot_check_test ( DistFunc_noncentralchisquare )
ot_check_test ( DistFunc_noncentralstudent )
ot_check_test ( DistFunc_normal )
ot_check_test ( DistFunc_poisson )
ot_check_test ( DistFunc_student )
if ( R_base_FOUND )
ot_check_test ( Distributions_draw )
endif ()

# Model
ot_check_test ( RandomVector_constant )
ot_check_test ( RandomVector_conditional )
ot_check_test ( RandomVector_distribution )

# Process
ot_check_test ( WhiteNoise_std )
ot_check_test ( RandomWalk_std )
ot_check_test ( ARMACoefficients_std )
ot_check_test ( ARMAState_std )
ot_check_test ( ARMA_std )
ot_check_test ( TemporalNormalProcess_std )
ot_check_test ( SpectralNormalProcess_std )
ot_check_test ( CompositeProcess_std  )
ot_check_test ( FunctionalBasisProcess_std  )
ot_check_test ( WhittleFactory_std )
# ot_check_test ( ARMALikelihoodFactory_std )

# Transformation
ot_check_test ( MarginalTransformationEvaluation_std )
ot_check_test ( MarginalTransformationGradient_std )
ot_check_test ( MarginalTransformationHessian_std )
ot_check_test ( InverseNatafIndependentCopulaEvaluation_std )
ot_check_test ( InverseNatafIndependentCopulaGradient_std )
ot_check_test ( InverseNatafIndependentCopulaHessian_std )
ot_check_test ( NatafIndependentCopulaEvaluation_std )
ot_check_test ( NatafIndependentCopulaGradient_std )
ot_check_test ( NatafIndependentCopulaHessian_std )
ot_check_test ( IsoProbabilisticTransformation_IndependentCopula )
ot_check_test ( IsoProbabilisticTransformation_EllipticalCopula )
ot_check_test ( IsoProbabilisticTransformation_EllipticalDistribution )
ot_check_test ( BoxCoxFactory_std )
ot_check_test ( TrendFactory_std )

# Bayesian
ot_check_test ( CalibrationStrategy_std )
ot_check_test ( RandomWalkMetropolisHastings_std )
ot_check_test ( RandomWalkMetropolisHastings_regression )
ot_check_test ( PosteriorRandomVector_std )

# Classification
ot_check_test ( MixtureClassifier_std )

# Experiments
ot_check_test ( MinMax_computation )
ot_check_test ( Axial_std )
ot_check_test ( Box_std )
ot_check_test ( Composite_std )
ot_check_test ( Factorial_std )
ot_check_test ( BootstrapExperiment_std )
ot_check_test ( FixedExperiment_std )
ot_check_test ( GaussProductExperiment_std )
ot_check_test ( ImportanceSamplingExperiment_std )
ot_check_test ( LHSExperiment_std )
ot_check_test ( LowDiscrepancyExperiment_std )
ot_check_test ( MonteCarloExperiment_std )

# OrthogonalBasis
ot_check_test ( LinearEnumerateFunction_std )
ot_check_test ( HyperbolicAnisotropicEnumerateFunction_std )
ot_check_test ( OrthogonalUniVariatePolynomial_std )
ot_check_test ( CharlierFactory_std )
ot_check_test ( HermiteFactory_std )
ot_check_test ( JacobiFactory_std )
ot_check_test ( KrawtchoukFactory_std )
ot_check_test ( LaguerreFactory_std )
ot_check_test ( LegendreFactory_std )
ot_check_test ( MeixnerFactory_std )
ot_check_test ( OrthonormalizationAlgorithm_std )
ot_check_test ( ChebychevAlgorithm_std )
ot_check_test ( GramSchmidtAlgorithm_std )
ot_check_test ( StandardDistributionPolynomialFactory_std )
ot_check_test ( OrthogonalBasis_std )

# MetaModel
ot_check_test ( FunctionalChaos_gsobol )
ot_check_test ( FunctionalChaos_ishigami )
ot_check_test ( FunctionalChaos_gsobol_sparse )
ot_check_test ( FunctionalChaos_ishigami_sparse )
ot_check_test ( FunctionalChaos_ishigami_database )
ot_check_test ( FunctionalChaos_nd )

# Simulation
ot_check_test ( Wilks_std )

# StatTests
if ( R_rot_FOUND )
ot_check_test ( VisualTest_std )
ot_check_test ( FittingTest_std )
ot_check_test ( HypothesisTest_std )
# ot_check_test ( HypothesisTest_correlation )
ot_check_test ( LinearModelTest_std )
ot_check_test ( NormalityTest_std )
endif ()
#ot_check_test ( DickeyFullerTest_std )

## Post-installation tests

# Common
ot_installcheck_test ( Path_std IGNOREOUT )

# Func
ot_installcheck_test ( LibraryLoader_load IGNOREOUT )
ot_installcheck_test ( Library_std IGNOREOUT )
ot_installcheck_test ( Library_cpp IGNOREOUT )
ot_installcheck_test ( ComputedNumericalMathEvaluationImplementation_std )
ot_installcheck_test ( ComputedNumericalMathEvaluationImplementation_wrongarg )
ot_installcheck_test ( ComputedNumericalMathEvaluationImplementation_copy )
ot_installcheck_test ( ComputedNumericalMathEvaluationImplementation_state )
ot_installcheck_test ( NumericalMathFunction_std )
ot_installcheck_test ( NumericalMathFunction_exec )
ot_installcheck_test ( NumericalMathFunction_exec_bug63 )
ot_installcheck_test ( NumericalMathFunction_exec_external )
ot_installcheck_test ( NumericalMathFunction_exec_sample )
ot_installcheck_test ( NumericalMathFunction_exec_sample_no_retry )
ot_installcheck_test ( NumericalMathFunction_exec_threads )
ot_installcheck_test ( NumericalMathFunction_grad )
ot_installcheck_test ( NumericalMathFunction_hess )
ot_installcheck_test ( NumericalMathFunction_sample )
ot_installcheck_test ( NumericalMathFunction_timeseries )
ot_installcheck_test ( NumericalMathFunction_composition )
ot_installcheck_test ( NumericalMathFunction_minimal )
ot_installcheck_test ( NumericalMathFunction_wrapper_error )
ot_installcheck_test ( MergeRandomAndConstantInput )

# Stat
ot_installcheck_test ( NumericalSample_csv )
ot_installcheck_test ( NumericalSample_csv_notfound )

# Diff
ot_installcheck_test ( FiniteDifferenceStep_std )
ot_installcheck_test ( NonCenteredFiniteDifferenceGradient_std )
ot_installcheck_test ( CenteredFiniteDifferenceGradient_std )
ot_installcheck_test ( CenteredFiniteDifferenceHessian_std )

# Optim
ot_installcheck_test ( AbdoRackwitz_std )
ot_installcheck_test ( AbdoRackwitz_linear )
ot_installcheck_test ( AbdoRackwitz_nonlinear )
ot_installcheck_test ( Cobyla_std )
ot_installcheck_test ( Cobyla_linear )
ot_installcheck_test ( Cobyla_nonlinear )
ot_installcheck_test ( SQP_std )
ot_installcheck_test ( SQP_linear )
ot_installcheck_test ( SQP_nonlinear )
ot_installcheck_test ( NearestPointChecker_std )
ot_installcheck_test ( TNC_std )
ot_installcheck_test ( TNC_linear )
ot_installcheck_test ( TNC_nonlinear )

# Model
ot_installcheck_test ( Event_std )
ot_installcheck_test ( Event_domain )
ot_installcheck_test ( StandardEvent_std )
ot_installcheck_test ( RandomVector_function )

# Experiments
ot_installcheck_test ( QuadraticTaylor_std )
ot_installcheck_test ( LinearTaylor_std )
ot_installcheck_test ( QuadraticLeastSquares_std )
ot_installcheck_test ( LinearLeastSquares_std )

# QuadraticCumul
ot_installcheck_test ( QuadraticCumul_std )
ot_installcheck_test ( QuadraticCumul_run )
ot_installcheck_test ( QuadraticCumul_importanceFactors )

# Analytical
ot_installcheck_test ( Analytical_std )
ot_installcheck_test ( AnalyticalResult_std )
ot_installcheck_test ( FORM_std )
ot_installcheck_test ( FORM_sensitivity )
if ( R_base_FOUND )
ot_installcheck_test ( FORM_draw )
endif ()
ot_installcheck_test ( FORM_SQP )
ot_installcheck_test ( FORM_AbdoRackwitz )
ot_installcheck_test ( SORM_std )
ot_installcheck_test ( StrongMaximumTest_std )
ot_installcheck_test ( StrongMaximumTest_tabulatedValues )
ot_installcheck_test ( StrongMaximumTest_other )

# Simulation
ot_installcheck_test ( MonteCarlo_std )
ot_installcheck_test ( MonteCarlo_threads )
if ( R_base_FOUND )
ot_installcheck_test ( MonteCarlo_draw )
endif ()
ot_installcheck_test ( MonteCarlo_sensitivity )
if ( R_base_FOUND )
ot_installcheck_test ( SimulationSensitivityAnalysis_std )
endif ()
ot_installcheck_test ( QuasiMonteCarlo_std )
ot_installcheck_test ( RandomizedQuasiMonteCarlo_std )
ot_installcheck_test ( DirectionalSampling_std )
ot_installcheck_test ( ImportanceSampling_std )
ot_installcheck_test ( PostAnalyticalControlledImportanceSampling_std )
ot_installcheck_test ( PostAnalyticalImportanceSampling_std )
ot_installcheck_test ( LHS_std )
ot_installcheck_test ( RandomizedLHS_std )

# Waarts tests
# ot_installcheck_test ( Waarts_25_quadratic_terms )
# ot_installcheck_test ( Waarts_concave )
# ot_installcheck_test ( Waarts_convex )
# ot_installcheck_test ( Waarts_discontinuous_lsf )
# ot_installcheck_test ( Waarts_noisy_lsf )
# ot_installcheck_test ( Waarts_oblate )
# ot_installcheck_test ( Waarts_RS2 )
# ot_installcheck_test ( Waarts_saddle )
# ot_installcheck_test ( Waarts_system_series )



# This macro creates a wrapper from the source file of the same name (suffixed by .cxx)
# If the source file has a different name or if there are many source files, they all
# must be listed as optional arguments.
# The macro installs the wrapper in its destination as well as the description file
# (named as the wrapper suffixed with .xml). A different name can be specified with
# the DESCRIPTION argument.
# ot_add_wrapper ( wrappername [ DESCRIPTION xmlfile ] [ SOURCES sourcefile ... ] )
macro ( ot_add_wrapper wrappername )
  set ( WRAPPERSOURCES )
  set ( WRAPPERDESCRIPTION )

  if ( ${ARGC} EQUAL 1 )
    set ( WRAPPERSOURCES ${wrappername}.cxx )
    set ( WRAPPERDESCRIPTION ${wrappername}.xml )

  else ( ${ARGC} EQUAL 1 )
    list ( INSERT WRAPPERSOURCES 0 ${ARGN} )
    list ( LENGTH WRAPPERSOURCES LEN )
    list ( GET    WRAPPERSOURCES 0 _ARGV0)
    string ( TOUPPER ${_ARGV0} _ARG )
    if ( ${_ARG} MATCHES DESCRIPTION )
      if ( ${LEN} EQUAL 1 )
        message ( FATAL_ERROR "Macro 'ot_add_wrapper( ${wrappername} ... )' has incorrect DESCRIPTION argument" )
      endif ( ${LEN} EQUAL 1 )
      if ( ${LEN} EQUAL 2 )
        list ( GET    WRAPPERSOURCES 1 _ARGV1)
        set ( WRAPPERDESCRIPTION ${_ARGV1} )
        set ( WRAPPERSOURCES ${wrappername}.cxx )
      else ( ${LEN} EQUAL 2 )
        list ( GET    WRAPPERSOURCES 1 _ARGV1)
        set ( WRAPPERDESCRIPTION ${_ARGV1} )
        list ( REMOVE_AT WRAPPERSOURCES 0 1 )
      endif ( ${LEN} EQUAL 2 )

    else ( ${_ARG} MATCHES DESCRIPTION )
      set ( WRAPPERDESCRIPTION ${wrappername}.xml )
    endif ( ${_ARG} MATCHES DESCRIPTION )
  endif ( ${ARGC} EQUAL 1 )

  add_library ( ${wrappername} MODULE ${WRAPPERSOURCES} )
  set_target_properties ( ${wrappername} PROPERTIES PREFIX "" )
  if ( MINGW )
    set_target_properties ( ${wrappername} PROPERTIES
                                           LINK_FLAGS "-shared -Wl,--enable-auto-import" )
    set_target_properties ( ${wrappername} PROPERTIES
                                           SUFFIX -0.dll )

    add_custom_target ( ${wrappername}_link ALL
      ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/${wrappername}-0.dll ${CMAKE_CURRENT_BINARY_DIR}/${wrappername}.so
      DEPENDS ${wrappername}
      COMMENT "Add a symbolic link to ${wrappername}."
    )
    install ( FILES ${CMAKE_CURRENT_BINARY_DIR}/${wrappername}.so
              DESTINATION ${OPENTURNS_WRAPPER_PATH}
              PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
              OPTIONAL
            )
    target_link_libraries ( ${wrappername} pthreadGC2 )
  endif ( MINGW )
  target_link_libraries ( ${wrappername} OTbind )
  install ( TARGETS ${wrappername}
            DESTINATION ${OPENTURNS_WRAPPER_PATH}
          )
  install ( FILES ${WRAPPERDESCRIPTION} 
            DESTINATION ${OPENTURNS_WRAPPER_PATH}
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
            OPTIONAL
          )
endmacro ( ot_add_wrapper )

ot_add_wrapper ( testwrapper_1 fonction_1.c )
ot_add_wrapper ( testwrapper_2 fonction_2.cxx )
ot_add_wrapper ( testwrapper_3 DESCRIPTION wrapper.xml wrapper_fonction1.cxx )
ot_add_wrapper ( poutre )
ot_add_wrapper ( poutre_fullspeed )
ot_add_wrapper ( poutre_sample )
ot_add_wrapper ( poutre_sample_no_retry )
ot_add_wrapper ( TestResponseSurface )
ot_add_wrapper ( TestQuadraticCumul )
ot_add_wrapper ( TestIdentity )
ot_add_wrapper ( TestCompositionLeft )
ot_add_wrapper ( TestCompositionRight )
ot_add_wrapper ( TestOptimLinear )
ot_add_wrapper ( TestOptimNonLinear )
ot_add_wrapper ( TestStrongMaximumTestDimension2 )
ot_add_wrapper ( TestStrongMaximumTestDimension5 )
ot_add_wrapper ( TestStrongMaximumTestDimension10 )
ot_add_wrapper ( TestStrongMaximumTest )
ot_add_wrapper ( TestStrongMaximumTest2 )
ot_add_wrapper ( minimal_wrapper )
ot_add_wrapper ( generic )
ot_add_wrapper ( external_code_threads )

install ( FILES poutre_external_LINUX.xml poutre_external_WIN32.xml
          DESTINATION ${OPENTURNS_WRAPPER_PATH}
          PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
        )

install ( FILES sample_good.csv
                sample_good_coma.csv
                sample_bad.csv
                sample_missing.csv
          DESTINATION ${OPENTURNS_SAMPLE_PATH}
          PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
        )

if ( MINGW )
  install ( PROGRAMS poutre_files.bat
            DESTINATION ${OPENTURNS_EXAMPLE_PATH}
          )

else ( MINGW )
  install ( PROGRAMS poutre_files.sh poutre_arguments.sh
            DESTINATION ${OPENTURNS_EXAMPLE_PATH}
          )
endif ( MINGW )
#add_custom_target ( check
#                    COMMAND ${CMAKE_CTEST_COMMAND}
#                  #           -VV
#                  #           --build-two-config
#                            --build-and-test ${CMAKE_CURRENT_SOURCE_DIR}/CheckTests ${CMAKE_CURRENT_BINARY_DIR}/CheckTests
#                            --build-generator ${CMAKE_GENERATOR}
#                            --build-makeprogram ${CMAKE_MAKE_PROGRAM}
#                            --build-options -DINTERNAL_INCLUDE_DIRS:INTERNAL="${INTERNAL_INCLUDE_DIRS}"
#                                            -DINTERNAL_LIBRARY_DIRS:INTERNAL="${OPENTURNS_LIBRARY_BUILD_DIR}"
#					    -DCHECK_TO_BE_RUN:INTERNAL="${CHECK_TO_BE_RUN}"
#                  #                           -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
#                            --test-command ${CMAKE_CURRENT_SOURCE_DIR}/run_check
#                  #  DEPENDS run_check
#                  #  DEPENDS ${CHECK_TO_BE_RUN}
#		  )

add_custom_target ( cppcheck COMMAND ${CMAKE_CTEST_COMMAND} -R "^cppcheck_"
                    DEPENDS ${CHECK_TO_BE_RUN}
                    COMMENT "Run C++ pre-installation tests" )

add_custom_target ( cppinstallcheck COMMAND ${CMAKE_CTEST_COMMAND} -R "^cppinstallcheck_"
                    DEPENDS ${INSTALLCHECK_TO_BE_RUN}
                    COMMENT "Run C++ post-installation tests" )

#get_cmake_property ( VARS VARIABLES )
#foreach ( var ${VARS} )
#  file ( APPEND ${CMAKE_CURRENT_BINARY_DIR}/VARS.txt
#        "${var} \"${${var}}\"\n" )
#endforeach ( var )



if ( MINGW )
  finish_check_file ( ${EXTERNAL_CHECK_FILE} ${CUR_CHECK} )
  finish_check_file ( ${EXTERNAL_INSTALLCHECK_FILE} ${CUR_INSTALLCHECK} )

  install ( FILES ${EXTERNAL_CHECK_FILE} ${EXTERNAL_INSTALLCHECK_FILE}
            DESTINATION ${EXAMPLE_PATH} 
            PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
          )

endif ( MINGW )

