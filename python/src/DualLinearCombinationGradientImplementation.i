// SWIG file DualLinearCombinationGradientImplementation.i
// @author dutka
// @date   2009-11-09 17:38:15 +0100 (lun. 09 nov. 2009)

%{
#include "DualLinearCombinationGradientImplementation.hxx"
%}

%include DualLinearCombinationGradientImplementation.hxx
namespace OT { %extend DualLinearCombinationGradientImplementation { DualLinearCombinationGradientImplementation(const DualLinearCombinationGradientImplementation & other) { return new OT::DualLinearCombinationGradientImplementation(other); } } }
