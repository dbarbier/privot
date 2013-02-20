// SWIG file SamplingStrategyImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "SamplingStrategyImplementation.hxx"
%}

%include SamplingStrategyImplementation.hxx
namespace OT{ %extend SamplingStrategyImplementation { SamplingStrategyImplementation(const SamplingStrategyImplementation & other) { return new OT::SamplingStrategyImplementation(other); } } }
