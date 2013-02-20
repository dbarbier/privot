// SWIG file Staircase.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Staircase.hxx"
%}

%include Staircase.hxx
namespace OT { %extend Staircase { Staircase(const Staircase & other) { return new OT::Staircase(other); } } }
