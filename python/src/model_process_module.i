// SWIG file model_process_module.i
// @author schueller
// @date   2011-06-01 18:20:26 +0200 (mer. 01 juin 2011)

%module(package="openturns", docstring="Stochastic process.") model_process
#pragma SWIG nowarn=302,509
%feature("autodoc","1");

%{
#include "OTconfig.hxx"
#include "OTBase.hxx"
#include "OTModel.hxx"
#include "OTDistribution.hxx"
#include "OTProcess.hxx"
%}

%include typemaps.i
%include OTtypes.i
%include OTexceptions.i
%include std_vector.i

%import common_module.i
%import typ_module.i
%include TestResult.i
%import base_module.i
%import dist_bundle1_module.i
%import dist_bundle2_module.i
%import model_copula_module.i
%import randomvector_module.i

/* Uncertainty/Process */
%include WhiteNoise.i
%include RandomWalk.i
%include ARMACoefficients.i
%include ARMAState.i
%include ARMA.i
%include SpectralNormalProcess.i
%include TemporalNormalProcess.i
%include CompositeProcess.i
%include FunctionalBasisProcess.i
%include ARMAFactoryImplementation.i
%include WhittleFactoryState.i
%include WhittleFactory.i
%include ARMALikelihoodFactory.i
%include ARMAFactory.i

/* At last we include template definitions */
%include UncertaintyProcessTemplateDefs.i
