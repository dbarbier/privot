// SWIG file ProductNumericalMathGradientImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ProductNumericalMathGradientImplementation.hxx"
%}

%include ProductNumericalMathGradientImplementation.hxx
namespace OT { %extend ProductNumericalMathGradientImplementation { ProductNumericalMathGradientImplementation(const ProductNumericalMathGradientImplementation & other) { return new OT::ProductNumericalMathGradientImplementation(other); } } }
