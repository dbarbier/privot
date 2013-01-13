// SWIG file RungeKutta.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "RungeKutta.hxx"
%}

%include RungeKutta.hxx
namespace OT { %extend RungeKutta { RungeKutta(const RungeKutta & other) { return new OT::RungeKutta(other); } } }
