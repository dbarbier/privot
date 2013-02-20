// SWIG file Secant.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Secant.hxx"
%}

%include Secant.hxx
namespace OT { %extend Secant { Secant(const Secant & other) { return new OT::Secant(other); } } }
