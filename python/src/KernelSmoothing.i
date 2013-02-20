// SWIG file KernelSmoothing.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "KernelSmoothing.hxx"
%}

%include KernelSmoothing.hxx
namespace OT { %extend KernelSmoothing { KernelSmoothing(const KernelSmoothing & other) { return new OT::KernelSmoothing(other); } } }
