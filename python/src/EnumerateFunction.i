// SWIG file EnumerateFunction.i
// @author schueller
// @date   2010-12-15 18:09:41 +0100 (Wed, 15 Dec 2010)

%{
#include "EnumerateFunction.hxx"
%}

OTTypedInterfaceObjectHelper(EnumerateFunction)

%include EnumerateFunction.hxx
namespace OT { %extend EnumerateFunction { EnumerateFunction(const EnumerateFunction & other) { return new OT::EnumerateFunction(other); } } }
