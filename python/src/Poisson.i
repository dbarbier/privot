// SWIG file Poisson.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Poisson.hxx"
%}

%include Poisson.hxx
namespace OT { %extend Poisson { Poisson(const Poisson & other) { return new OT::Poisson(other); } } }
