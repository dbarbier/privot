// SWIG file ProductNumericalMathHessianImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ProductNumericalMathHessianImplementation.hxx"
%}

%include ProductNumericalMathHessianImplementation.hxx
namespace OT { %extend ProductNumericalMathHessianImplementation { ProductNumericalMathHessianImplementation(const ProductNumericalMathHessianImplementation & other) { return new OT::ProductNumericalMathHessianImplementation(other); } } }
