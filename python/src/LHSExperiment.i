// SWIG file LHSExperiment.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LHSExperiment.hxx"
%}

%include LHSExperiment.hxx
namespace OT { %extend LHSExperiment { LHSExperiment(const LHSExperiment & other) { return new OT::LHSExperiment(other); } } }
