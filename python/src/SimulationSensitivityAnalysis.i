// SWIG file SimulationSensitivityAnalysis.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (lun. 02 janv. 2012)

%{
#include "SimulationSensitivityAnalysis.hxx"
%}

%include SimulationSensitivityAnalysis.hxx
namespace OT{ %extend SimulationSensitivityAnalysis { SimulationSensitivityAnalysis(const SimulationSensitivityAnalysis & other) { return new OT::SimulationSensitivityAnalysis(other); } } }
