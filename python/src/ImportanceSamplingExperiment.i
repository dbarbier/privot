// SWIG file ImportanceSamplingExperiment.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ImportanceSamplingExperiment.hxx"
%}

%include ImportanceSamplingExperiment.hxx
namespace OT { %extend ImportanceSamplingExperiment { ImportanceSamplingExperiment(const ImportanceSamplingExperiment & other) { return new OT::ImportanceSamplingExperiment(other); } } }
