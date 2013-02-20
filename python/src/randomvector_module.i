// SWIG file randomvector_module.i
// @author schueller
// @date   2011-11-04 15:00:58 +0100 (Fri, 04 Nov 2011)

%module(package="openturns", docstring="Random vectors") randomvector
#pragma SWIG nowarn=302,509
%feature("autodoc","1");

%{
#include "OTconfig.hxx"
#include "OTBase.hxx"
#include "OTModel.hxx"
#include "OTDistribution.hxx"
%}

%include typemaps.i
%include OTtypes.i
%include OTexceptions.i
%include std_vector.i

%import common_module.i
%import typ_module.i
%include TestResult.i
%import base_module.i
%import model_copula_module.i

/* Uncertainty/Model */
%include ProcessImplementation.i
%include Process.i
%include RandomVectorImplementation.i
%include ConstantRandomVector.i
%include UsualRandomVector.i

/* At last we include template definitions */
%include UncertaintyRandomVectorTemplateDefs.i
