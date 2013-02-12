// SWIG file CombinatorialGeneratorImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "CombinatorialGeneratorImplementation.hxx"
%}

%include CombinatorialGeneratorImplementation.hxx
namespace OT { %extend CombinatorialGeneratorImplementation { CombinatorialGeneratorImplementation(const CombinatorialGeneratorImplementation & other) { return new OT::CombinatorialGeneratorImplementation(other); } } }
