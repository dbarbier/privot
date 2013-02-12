// SWIG file CombinatorialGenerator.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "CombinatorialGenerator.hxx"
%}

OTTypedInterfaceObjectHelper(CombinatorialGenerator)

%include CombinatorialGenerator.hxx
namespace OT { %extend CombinatorialGenerator { CombinatorialGenerator(const CombinatorialGenerator & other) { return new OT::CombinatorialGenerator(other); } } }
