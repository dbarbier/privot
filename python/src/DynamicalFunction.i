// SWIG file DynamicalFunction.i
// @author lebrun
// @date   2011-08-01 17:35:56 +0200 (lun. 01 août 2011)

%{
#include "DynamicalFunction.hxx"
%}

OTTypedInterfaceObjectHelper(DynamicalFunction)

%include DynamicalFunction.hxx

namespace OT { %extend DynamicalFunction { DynamicalFunction(const DynamicalFunction & other) { return new OT::DynamicalFunction( other ); } } }
