// SWIG file SolverImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "SolverImplementation.hxx"
%}

%include SolverImplementation.hxx
namespace OT { %extend SolverImplementation { SolverImplementation(const SolverImplementation & other) { return new OT::SolverImplementation(other); } } }
