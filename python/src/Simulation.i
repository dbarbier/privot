// SWIG file Simulation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Simulation.hxx"
%}

%include Simulation.hxx

namespace OT{ %extend Simulation { Simulation(const Simulation & other) { return new OT::Simulation(other); } } }
