// SWIG file ChiSquare.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ChiSquare.hxx"
%}

%include ChiSquare.hxx
namespace OT { %extend ChiSquare { ChiSquare(const ChiSquare & other) { return new OT::ChiSquare(other); } } }
