// SWIG file classification_module.i
// @author dutka
// @date   2011-11-17 13:46:13 +0100 (Thu, 17 Nov 2011)

%module(package="openturns", docstring="Classification algorithms.") classification
#pragma SWIG nowarn=302,509
%feature("autodoc","1");

%{
#include "OTconfig.hxx"
#include "OTBase.hxx"
#include "OTModel.hxx"
#include "OTClassification.hxx"
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
%import  model_copula_module.i
%import UncertaintyModelCopulaTemplateDefs.i
//%import randomvector_module.i
%import dist_bundle1_module.i
%import dist_bundle2_module.i

/* Uncertainty/Algorithm/Classification */
%include MixtureClassifier.i

/* At last we include template definitions */
%include UncertaintyClassificationTemplateDefs.i
