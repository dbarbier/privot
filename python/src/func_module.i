// SWIG file func_module.i
// @author schueller
// @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)

%module(package="openturns", docstring="Function primitives.") func
#pragma SWIG nowarn=302,314,509
%feature("autodoc","1");

%{
#include "OTconfig.hxx"
#include "OTCommon.hxx"
#include "OTType.hxx"
#include "OTStat.hxx"
#include "OTGraph.hxx"
#include "OTFunc.hxx"
#include "OTGeom.hxx"
#include "OTAlgo.hxx"
%}

%include typemaps.i
%include OTtypes.i
%include OTexceptions.i
%include std_vector.i

/* Base/Common */
%import common_module.i
%import BaseCommonTemplateDefs.i

/* Wrapper */
%import wrapper_module.i
%import BaseWrapperTemplateDefs.i

/* Base/Type */
%import typ_module.i
%import BaseTypTemplateDefs.i

/* Base/Geom */
%import geom_module.i
%import BaseGeomTemplateDefs.i

/* Base/Func */
%include TimeSeries.i 
%include ProcessSample.i 
%include SpecFunc.i
%include NumericalMathEvaluationImplementation.i
%include NumericalMathGradientImplementation.i
%include NumericalMathHessianImplementation.i
%include NoNumericalMathEvaluationImplementation.i
%include NoNumericalMathGradientImplementation.i
%include NoNumericalMathHessianImplementation.i
%include LinearNumericalMathEvaluationImplementation.i
%include LinearNumericalMathGradientImplementation.i
%include QuadraticNumericalMathEvaluationImplementation.i
%include ConstantNumericalMathGradientImplementation.i
%include ConstantNumericalMathHessianImplementation.i
%include ComputedNumericalMathEvaluationImplementation.i
%include ComputedNumericalMathGradientImplementation.i
%include ComputedNumericalMathHessianImplementation.i
%include ComposedNumericalMathEvaluationImplementation.i
%include ComposedNumericalMathGradientImplementation.i
%include ComposedNumericalMathHessianImplementation.i
%include ProductNumericalMathEvaluationImplementation.i
%include ProductNumericalMathGradientImplementation.i
%include ProductNumericalMathHessianImplementation.i
%include NumericalMathFunctionImplementation.i
%include ProductNumericalMathFunction.i
%include NumericalMathFunction.i
%include DynamicalFunctionImplementation.i
%include DynamicalFunction.i
%include SpatialFunction.i
%include TemporalFunction.i
%include ComposedNumericalMathFunction.i
%include DualLinearCombinationEvaluationImplementation.i
%include DualLinearCombinationGradientImplementation.i
%include DualLinearCombinationHessianImplementation.i
%include LinearCombinationEvaluationImplementation.i
%include LinearCombinationGradientImplementation.i
%include LinearCombinationHessianImplementation.i
%include LinearNumericalMathFunction.i
%include UniVariatePolynomialImplementation.i
%include UniVariatePolynomial.i
%include AggregatedNumericalMathEvaluationImplementation.i
%include PiecewiseEvaluationImplementation.i
%include AnalyticalNumericalMathEvaluationImplementation.i
%include AnalyticalNumericalMathGradientImplementation.i
%include AnalyticalNumericalMathHessianImplementation.i
%include DatabaseNumericalMathEvaluationImplementation.i
%include IndicatorNumericalMathEvaluationImplementation.i
%include ProductPolynomialEvaluationImplementation.i
%include RestrictedEvaluationImplementation.i
%include RestrictedGradientImplementation.i
%include RestrictedHessianImplementation.i
%include Basis.i
%include BasisSequenceImplementation.i
%include BasisSequence.i
%include BasisSequenceFactoryImplementation.i
%include BasisSequenceFactory.i
%include LAR.i
%include LARLasso.i
%include LARFSZero.i
%include BoxCoxEvaluationImplementation.i
%include BoxCoxTransform.i
%include InverseBoxCoxEvaluationImplementation.i
%include InverseBoxCoxTransform.i
%include TrendEvaluationImplementation.i
%include TrendTransform.i
%include InverseTrendEvaluationImplementation.i
%include InverseTrendTransform.i

/* At last we include template definitions */
%include BaseFuncTemplateDefs.i
