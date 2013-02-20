// SWIG file Mixture.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Mixture.hxx"
%}

%include Mixture.hxx
namespace OT { %extend Mixture { Mixture(const Mixture & other) { return new OT::Mixture(other); } } }
