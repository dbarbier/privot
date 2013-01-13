// SWIG file ODESolver.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ODESolver.hxx"
%}

OTTypedInterfaceObjectHelper(ODESolver)

%include ODESolver.hxx
namespace OT { %extend ODESolver { ODESolver(const ODESolver & other) { return new OT::ODESolver(other); } } }
