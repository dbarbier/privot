// SWIG file RandomVectorImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "RandomVectorImplementation.hxx"
%}

%include RandomVectorImplementation.hxx
namespace OT { %extend RandomVectorImplementation { RandomVectorImplementation(const RandomVectorImplementation & other) { return new OT::RandomVectorImplementation(other); } } }
