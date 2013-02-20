// SWIG file SpecFunc.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "SpecFunc.hxx"
%}

%include SpecFunc.hxx
namespace OT { %extend SpecFunc { SpecFunc(const SpecFunc & other) { return new OT::SpecFunc(other); } } }
