// SWIG file CorrelationAnalysis.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "CorrelationAnalysis.hxx"
%}

%include CorrelationAnalysis.hxx

namespace OT { %extend CorrelationAnalysis { CorrelationAnalysis(const CorrelationAnalysis & other) { return new OT::CorrelationAnalysis(other); } } }
