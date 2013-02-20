// SWIG file NearestPointChecker.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NearestPointChecker.hxx"
%}

%include NearestPointChecker.hxx
namespace OT{ %extend NearestPointChecker { NearestPointChecker(const NearestPointChecker & other) { return new OT::NearestPointChecker(other); } } }
