// SWIG file AnalyticalNumericalMathHessianImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (lun. 02 janv. 2012)

%{
#include "AnalyticalNumericalMathHessianImplementation.hxx"
%}

%include AnalyticalNumericalMathHessianImplementation.hxx
namespace OT { %extend AnalyticalNumericalMathHessianImplementation { AnalyticalNumericalMathHessianImplementation(const AnalyticalNumericalMathHessianImplementation & other) { return new OT::AnalyticalNumericalMathHessianImplementation(other); } } }
