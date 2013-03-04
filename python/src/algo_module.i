// SWIG file algo_module.i
// @author schueller
// @date   2010-11-09 13:44:00 +0100 (Tue, 09 Nov 2010)

%module(package="openturns", docstring="Approximation algorithms.") algo
%feature("autodoc","1");

%{
#include "OTconfig.hxx"
#include "OTAlgo.hxx"
#include "OTStat.hxx"
#include "OTFunc.hxx"
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

/* Base/Func */
%include BaseFuncCollection.i
%import func_module.i
%import BaseFuncTemplateDefs.i

/* Base/Stat */
%import statistics_module.i
%import BaseStatisticsTemplateDefs.i

/* Base/Algo */
%include ApproximationAlgorithmImplementation.i
%include ApproximationAlgorithm.i
%include ApproximationAlgorithmImplementationFactory.i
%include ClassifierImplementation.i
%include Classifier.i
%include FittingAlgorithmImplementation.i
%include FittingAlgorithm.i
%include KFold.i
%include CorrectedLeaveOneOut.i
%include Lasso.i
%include LassoFactory.i
%include LeastSquaresMetaModelSelection.i
%include LeastSquaresMetaModelSelectionFactory.i
%include PenalizedLeastSquaresAlgorithm.i
%include PenalizedLeastSquaresAlgorithmFactory.i
%include KissFFT.i
%include ExpertMixture.i

/* At last we include template definitions */
%include BaseAlgoTemplateDefs.i
