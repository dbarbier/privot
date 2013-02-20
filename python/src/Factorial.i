// SWIG file Factorial.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Factorial.hxx"
%}

%include Factorial.hxx
namespace OT { %extend Factorial { Factorial(const Factorial & other) { return new OT::Factorial(other); } } }
