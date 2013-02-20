// SWIG file ChebychevAlgorithm.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ChebychevAlgorithm.hxx"
%}

%include ChebychevAlgorithm.hxx
namespace OT{ %extend ChebychevAlgorithm { ChebychevAlgorithm(const ChebychevAlgorithm & other) { return new OT::ChebychevAlgorithm(other); } } }
