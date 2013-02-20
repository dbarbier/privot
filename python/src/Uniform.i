// SWIG file Uniform.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Uniform.hxx"
%}

%include Uniform.hxx
namespace OT { %extend Uniform { Uniform(const Uniform & other) { return new OT::Uniform(other); } } }
