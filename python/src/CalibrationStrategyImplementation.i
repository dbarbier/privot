// SWIG file CalibrationStrategyImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "CalibrationStrategyImplementation.hxx"
%}

%include CalibrationStrategyImplementation.hxx
namespace OT { %extend CalibrationStrategyImplementation { CalibrationStrategyImplementation(const CalibrationStrategyImplementation & other) { return new OT::CalibrationStrategyImplementation(other); } } }
