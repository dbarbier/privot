// SWIG file Solver.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Solver.hxx"
%}

OTTypedInterfaceObjectHelper(Solver)

%include Solver.hxx
namespace OT { %extend Solver { Solver(const Solver & other) { return new OT::Solver(other); } } }
