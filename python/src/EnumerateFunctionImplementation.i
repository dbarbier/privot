// SWIG file EnumerateFunctionImplementation.i
// @author schueller
// @date   2010-12-15 18:09:41 +0100 (Wed, 15 Dec 2010)

%{
#include "EnumerateFunctionImplementation.hxx"
%}

%include EnumerateFunctionImplementation.hxx
namespace OT { %extend EnumerateFunctionImplementation { EnumerateFunctionImplementation(const EnumerateFunctionImplementation & other) { return new OT::EnumerateFunctionImplementation(other); } } }
