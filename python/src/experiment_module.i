// SWIG file experiment_module.i
// @author dutka
// @date   2011-11-17 13:46:13 +0100 (Thu, 17 Nov 2011)

%module(package="openturns", docstring="Designs of experiments.") experiment
#pragma SWIG nowarn=302,509
%feature("autodoc","1");

%{
#include "OTconfig.hxx"
#include "OTBase.hxx"
#include "OTExperiments.hxx"
%}

%include typemaps.i
%include OTtypes.i
%include OTexceptions.i
%include std_vector.i

%import common_module.i
%import typ_module.i
%import statistics_module.i

/* Uncertainty/Algorithm/Experiment */
%include ExperimentImplementation.i
%include StratifiedExperiment.i
%include Experiment.i
%include Axial.i
%include Composite.i
%include Factorial.i
%include Box.i
%include CombinatorialGeneratorImplementation.i
%include CombinatorialGenerator.i
%include Combinations.i
%include KPermutations.i
%include Tuples.i
