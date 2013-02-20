// SWIG file ChiSquareFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ChiSquareFactory.hxx"
%}

%include ChiSquareFactory.hxx
namespace OT { %extend ChiSquareFactory { ChiSquareFactory(const ChiSquareFactory & other) { return new OT::ChiSquareFactory(other); } } }
