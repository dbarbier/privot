// SWIG file Experiment.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Experiment.hxx"
%}

OTTypedInterfaceObjectHelper(Experiment)

%include Experiment.hxx
namespace OT { %extend Experiment { Experiment(const Experiment & other) { return new OT::Experiment(other); } } }
