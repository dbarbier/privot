// SWIG file Tuples.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Tuples.hxx"
%}

%include Tuples.hxx
namespace OT { %extend Tuples { Tuples(const Tuples & other) { return new OT::Tuples(other); } } }
