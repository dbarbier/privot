// SWIG file DynamicalFunctionImplementation.i
// @author dutka
// @date   2010-11-09 13:44:00 +0100 (mar. 09 nov. 2010)

%{
#include "DynamicalFunctionImplementation.hxx"
%}

%include DynamicalFunctionImplementation.hxx
namespace OT { %extend DynamicalFunctionImplementation { DynamicalFunctionImplementation(const DynamicalFunctionImplementation & other) { return new OT::DynamicalFunctionImplementation(other); } } }
