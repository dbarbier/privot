// SWIG file CopulaImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "CopulaImplementation.hxx"
%}

%include CopulaImplementation.hxx
namespace OT { %extend CopulaImplementation { CopulaImplementation(const CopulaImplementation & other) { return new OT::CopulaImplementation(other); } } }
