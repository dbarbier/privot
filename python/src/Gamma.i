// SWIG file Gamma.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Gamma.hxx"
%}

%include Gamma.hxx
namespace OT { %extend Gamma { Gamma(const Gamma & other) { return new OT::Gamma(other); } } }
