// SWIG file FiniteDifferenceStep.i
// @author dutka
// @date   2007-03-07 15:50:39 +0100 (mer. 07 mars 2007)

%{
#include "FiniteDifferenceStep.hxx"
%}

OTTypedInterfaceObjectHelper(FiniteDifferenceStep)

%include FiniteDifferenceStep.hxx
namespace OT { %extend FiniteDifferenceStep { FiniteDifferenceStep(const FiniteDifferenceStep & other) { return new OT::FiniteDifferenceStep(other); } } }
