// SWIG file SimulationResult.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "SimulationResult.hxx"
%}

OTTypedInterfaceObjectHelper(SimulationResult)

%include SimulationResult.hxx
namespace OT{ %extend SimulationResult { SimulationResult(const SimulationResult & other) { return new OT::SimulationResult(other); } } }
