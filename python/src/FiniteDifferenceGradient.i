// SWIG file FiniteDifferenceGradient.i
// @author dutka
// @date   2007-03-07 15:50:39 +0100 (mer. 07 mars 2007)

%{
#include "FiniteDifferenceGradient.hxx"
%}

%include FiniteDifferenceGradient.hxx
namespace OT { %extend FiniteDifferenceGradient { FiniteDifferenceGradient(const FiniteDifferenceGradient & other) { return new OT::FiniteDifferenceGradient(other); } } }
