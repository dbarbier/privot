// SWIG file NearestPointCheckerResult.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NearestPointCheckerResult.hxx"
%}

%include NearestPointCheckerResult.hxx
namespace OT{ %extend NearestPointCheckerResult { NearestPointCheckerResult(const NearestPointCheckerResult & other) { return new OT::NearestPointCheckerResult(other); } } }
