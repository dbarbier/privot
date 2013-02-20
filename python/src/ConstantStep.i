// SWIG file ConstantStep.i
// @author dutka
// @date   2007-03-07 15:50:39 +0100 (mer. 07 mars 2007)

%{
#include "ConstantStep.hxx"
%}

%include ConstantStep.hxx
namespace OT { %extend ConstantStep { ConstantStep(const ConstantStep & other) { return new OT::ConstantStep(other); } } }
