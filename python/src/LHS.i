// SWIG file LHS.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LHS.hxx"
%}

%include LHS.hxx
namespace OT{ %extend LHS { LHS(const LHS & other) { return new OT::LHS(other); } } }
