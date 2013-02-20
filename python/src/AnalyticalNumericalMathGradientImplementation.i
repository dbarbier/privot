// SWIG file AnalyticalNumericalMathGradientImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (lun. 02 janv. 2012)

%{
#include "AnalyticalNumericalMathGradientImplementation.hxx"
%}

%include AnalyticalNumericalMathGradientImplementation.hxx
namespace OT { %extend AnalyticalNumericalMathGradientImplementation { AnalyticalNumericalMathGradientImplementation(const AnalyticalNumericalMathGradientImplementation & other) { return new OT::AnalyticalNumericalMathGradientImplementation(other); } } }
