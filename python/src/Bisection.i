// SWIG file Bisection.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Bisection.hxx"
%}

%include Bisection.hxx
namespace OT { %extend Bisection { Bisection(const Bisection & other) { return new OT::Bisection(other); } } }
