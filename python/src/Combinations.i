// SWIG file Combinations.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Combinations.hxx"
%}

%include Combinations.hxx
namespace OT { %extend Combinations { Combinations(const Combinations & other) { return new OT::Combinations(other); } } }
