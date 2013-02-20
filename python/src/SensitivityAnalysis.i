// SWIG file SensitivityAnalysis.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "SensitivityAnalysis.hxx"
%}


%include SensitivityAnalysis.hxx
namespace OT{ %extend SensitivityAnalysis { SensitivityAnalysis(const SensitivityAnalysis & other) { return new OT::SensitivityAnalysis(other); } } }
