// SWIG file DistFunc.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "DistFunc.hxx"
%}

%include DistFunc.hxx
namespace OT { %extend DistFunc { DistFunc(const DistFunc & other) { return new OT::DistFunc(other); } } }
