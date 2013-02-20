// SWIG file SimulationResultImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "SimulationResultImplementation.hxx"
%}

%include SimulationResultImplementation.hxx
namespace OT{ %extend SimulationResultImplementation { SimulationResultImplementation(const SimulationResultImplementation & other) { return new OT::SimulationResultImplementation(other); } } }
