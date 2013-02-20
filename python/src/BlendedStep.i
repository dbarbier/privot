// SWIG file BlendedStep.i
// @author dutka
// @date   2007-03-07 15:50:39 +0100 (mer. 07 mars 2007)

%{
#include "BlendedStep.hxx"
%}

%include BlendedStep.hxx
namespace OT { %extend BlendedStep { BlendedStep(const BlendedStep & other) { return new OT::BlendedStep(other); } } }
