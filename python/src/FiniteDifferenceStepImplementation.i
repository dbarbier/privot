// SWIG file FiniteDifferenceStepImplementation.i
// @author dutka
// @date   2007-03-07 15:50:39 +0100 (mer. 07 mars 2007)

%{
#include "FiniteDifferenceStepImplementation.hxx"
%}

%include FiniteDifferenceStepImplementation.hxx
namespace OT { %extend FiniteDifferenceStepImplementation { FiniteDifferenceStepImplementation(const FiniteDifferenceStepImplementation & other) { return new OT::FiniteDifferenceStepImplementation(other); } } }
