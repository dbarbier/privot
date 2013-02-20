// SWIG file ExperimentImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ExperimentImplementation.hxx"
%}

%include ExperimentImplementation.hxx
namespace OT { %extend ExperimentImplementation { ExperimentImplementation(const ExperimentImplementation & other) { return new OT::ExperimentImplementation(other); } } }
