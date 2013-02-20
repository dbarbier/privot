// SWIG file LinearCombinationGradientImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LinearCombinationGradientImplementation.hxx"
%}

%include LinearCombinationGradientImplementation.hxx
namespace OT { %extend LinearCombinationGradientImplementation { LinearCombinationGradientImplementation(const LinearCombinationGradientImplementation & other) { return new OT::LinearCombinationGradientImplementation(other); } } }
