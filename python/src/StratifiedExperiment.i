// SWIG file StratifiedExperiment.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "StratifiedExperiment.hxx"
%}

%include StratifiedExperiment.hxx
namespace OT { %extend StratifiedExperiment { StratifiedExperiment(const StratifiedExperiment & other) { return new OT::StratifiedExperiment(other); } } }
