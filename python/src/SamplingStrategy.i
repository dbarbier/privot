// SWIG file SamplingStrategy.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "SamplingStrategy.hxx"
%}

OTTypedInterfaceObjectHelper(SamplingStrategy)

%include SamplingStrategy.hxx
namespace OT{ %extend SamplingStrategy { SamplingStrategy(const SamplingStrategy & other) { return new OT::SamplingStrategy(other); } } }
