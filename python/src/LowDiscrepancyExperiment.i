// SWIG file LowDiscrepancyExperiment.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LowDiscrepancyExperiment.hxx"
%}

%include LowDiscrepancyExperiment.hxx
namespace OT { %extend LowDiscrepancyExperiment { LowDiscrepancyExperiment(const LowDiscrepancyExperiment & other) { return new OT::LowDiscrepancyExperiment(other); } } }
