// SWIG file bayesian_module.i
// @author schueller
// @date   2012-11-17 13:46:13 +0100 (Thu, 17 Nov 2012)

%module(package="openturns", docstring="Bayesian algorithms.") bayesian
%feature("autodoc","1");

%{
#include "OTconfig.hxx"
#include "OTBase.hxx"
#include "OTModel.hxx"
#include "OTBayesian.hxx"
%}

%include typemaps.i
%include OTtypes.i
%include OTexceptions.i
%include std_vector.i

%import common_module.i
%import typ_module.i
%include TestResult.i
%import base_module.i

/* Uncertainty/Model */
/* Uncertainty/Distribution */
%include UncertaintyModelCopulaCollection.i
%import model_copula_module.i
%import UncertaintyModelCopulaTemplateDefs.i
%import dist_bundle1_module.i
%import dist_bundle2_module.i

/* Uncertainty/Model */
%import randomvector_module.i

/* Uncertainty/Bayesian */
%include CalibrationStrategyImplementation.i
%include CalibrationStrategy.i
%include SamplerImplementation.i
%include Sampler.i
%include MCMC.i
%include RandomWalkMetropolisHastings.i
%include PosteriorRandomVector.i

/* At last we include template definitions */
%include UncertaintyBayesianTemplateDefs.i
