// SWIG file LinearCombinationHessianImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LinearCombinationHessianImplementation.hxx"
%}

%include LinearCombinationHessianImplementation.hxx
namespace OT { %extend LinearCombinationHessianImplementation { LinearCombinationHessianImplementation(const LinearCombinationHessianImplementation & other) { return new OT::LinearCombinationHessianImplementation(other); } } }
