// SWIG file WeightedExperiment.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "WeightedExperiment.hxx"
%}

%include WeightedExperiment.hxx
namespace OT { %extend WeightedExperiment { WeightedExperiment(const WeightedExperiment & other) { return new OT::WeightedExperiment(other); } } }
