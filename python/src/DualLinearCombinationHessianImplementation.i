// SWIG file DualLinearCombinationHessianImplementation.i
// @author dutka
// @date   2009-11-09 17:38:15 +0100 (lun. 09 nov. 2009)

%{
#include "DualLinearCombinationHessianImplementation.hxx"
%}

%include DualLinearCombinationHessianImplementation.hxx
namespace OT { %extend DualLinearCombinationHessianImplementation { DualLinearCombinationHessianImplementation(const DualLinearCombinationHessianImplementation & other) { return new OT::DualLinearCombinationHessianImplementation(other); } } }
