// SWIG file DualLinearCombinationEvaluationImplementation.i
// @author dutka
// @date   2009-11-09 17:38:15 +0100 (lun. 09 nov. 2009)

%{
#include "DualLinearCombinationEvaluationImplementation.hxx"
%}

%include DualLinearCombinationEvaluationImplementation.hxx

namespace OT { %extend DualLinearCombinationEvaluationImplementation { DualLinearCombinationEvaluationImplementation(const DualLinearCombinationEvaluationImplementation & other) { return new OT::DualLinearCombinationEvaluationImplementation(other); } } }
