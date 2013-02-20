// SWIG file MonteCarloExperiment.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "MonteCarloExperiment.hxx"
%}

%include MonteCarloExperiment.hxx
namespace OT { %extend MonteCarloExperiment { MonteCarloExperiment(const MonteCarloExperiment & other) { return new OT::MonteCarloExperiment(other); } } }
