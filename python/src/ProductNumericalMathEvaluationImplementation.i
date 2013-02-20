// SWIG file ProductNumericalMathEvaluationImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ProductNumericalMathEvaluationImplementation.hxx"
%}

%include ProductNumericalMathEvaluationImplementation.hxx
namespace OT { %extend ProductNumericalMathEvaluationImplementation { ProductNumericalMathEvaluationImplementation(const ProductNumericalMathEvaluationImplementation & other) { return new OT::ProductNumericalMathEvaluationImplementation(other); } } }
