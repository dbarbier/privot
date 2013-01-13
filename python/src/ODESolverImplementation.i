// SWIG file ODESolverImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ODESolverImplementation.hxx"
%}

%include ODESolverImplementation.hxx
namespace OT { %extend ODESolverImplementation { ODESolverImplementation(const ODESolverImplementation & other) { return new OT::ODESolverImplementation(other); } } }
