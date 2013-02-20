// SWIG file ProductNumericalMathFunction.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ProductNumericalMathFunction.hxx"
%}

%include ProductNumericalMathFunction.hxx
namespace OT { %extend ProductNumericalMathFunction { ProductNumericalMathFunction(const ProductNumericalMathFunction & other) { return new OT::ProductNumericalMathFunction(other); } } }
