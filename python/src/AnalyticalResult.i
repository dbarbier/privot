// SWIG file AnalyticalResult.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "AnalyticalResult.hxx"
%}

%include AnalyticalResult.hxx
namespace OT{ %extend AnalyticalResult { AnalyticalResult(const AnalyticalResult & other) { return new OT::AnalyticalResult(other); } } }
