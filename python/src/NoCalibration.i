// SWIG file NoCalibration.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NoCalibration.hxx"
%}

%include NoCalibration.hxx
namespace OT { %extend NoCalibration { NoCalibration(const NoCalibration & other) { return new OT::NoCalibration(other); } } }
