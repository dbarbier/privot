// SWIG file FixedExperiment.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "FixedExperiment.hxx"
%}

%include FixedExperiment.hxx
namespace OT { %extend FixedExperiment { FixedExperiment(const FixedExperiment & other) { return new OT::FixedExperiment(other); } } }
