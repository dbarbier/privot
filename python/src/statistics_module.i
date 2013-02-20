// SWIG file statistics_module.i
// @author schueller
// @date   2012-01-04 12:26:21 +0100 (Wed, 04 Jan 2012)

%module(package="openturns", docstring="Statistical classes.") statistics
%feature("autodoc","1");

%{
#include "OTconfig.hxx"
#include "OTCommon.hxx"
#include "OTType.hxx"
#include "OTStat.hxx"
#include "OTGeom.hxx"
#include "OTFunc.hxx"
%}

%include typemaps.i
%include OTtypes.i
%include OTexceptions.i
%include std_vector.i

/* Base/Common */
%import common_module.i

/* Wrapper */
%import wrapper_module.i
%import BaseWrapperTemplateDefs.i

/* Base/Type */
%import typ_module.i
%import BaseTypTemplateDefs.i

/* Base/Graph */
%import graph_module.i
%import BaseGraphTemplateDefs.i

/* Base / Func */
%import func_module.i
%import BaseFuncTemplateDefs.i

/* Base/Stat */ 
%include Compact.i  
%include Full.i  
%include Last.i  
%include Null.i  
%include ConfidenceInterval.i
%include LinearModel.i  
%include LinearModelFactory.i  
%include TestResult.i  
%include RandomGenerator.i
%include LowDiscrepancySequenceImplementation.i 
%include LowDiscrepancySequence.i 
%include SobolSequence.i 
%include FaureSequence.i 
%include HaltonSequence.i 
%include HaselgroveSequence.i 
%include ReverseHaltonSequence.i 
%include CorrelationAnalysis.i 
%include SensitivityAnalysis.i 
%include FilteringWindowsImplementation.i
%include FilteringWindows.i
%include Hamming.i
%include Hanning.i
%include FFTImplementation.i
%include FFT.i
%include SpectralModelImplementation.i
%include SpectralModel.i
%include CovarianceModelImplementation.i
%include CovarianceModel.i
%include StationaryCovarianceModel.i
%include SecondOrderModelImplementation.i 
%include ExponentialCauchy.i 
%include SecondOrderModel.i 
%include SpectralModelFactoryImplementation.i
%include SpectralModelFactory.i 
%include UserDefinedSpectralModel.i
%include WelchFactory.i
%include ExponentialModel.i
%include CauchyModel.i
%include CovarianceModelFactoryImplementation.i
%include CovarianceModelFactory.i
%include UserDefinedCovarianceModel.i
%include NonStationaryCovarianceModelFactory.i
%include UserDefinedStationaryCovarianceModel.i
%include StationaryCovarianceModelFactory.i

/* At last we include template definitions */
%include BaseStatisticsTemplateDefs.i
