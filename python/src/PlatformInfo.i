// SWIG file PlatformInfo.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "PlatformInfo.hxx"
%}

%include PlatformInfo.hxx
namespace OT{ %extend PlatformInfo { PlatformInfo(const PlatformInfo & other) { return new OT::PlatformInfo(other); } } }
