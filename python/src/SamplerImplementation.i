// SWIG file SamplerImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "SamplerImplementation.hxx"
%}

%include SamplerImplementation.hxx
namespace OT { %extend SamplerImplementation { SamplerImplementation(const SamplerImplementation & other) { return new OT::SamplerImplementation(other); } } }
