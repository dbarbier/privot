// SWIG file Log.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Log.hxx"
%}

%apply UnsignedLong { OT::Log::Severity };

%include Log.hxx

namespace OT{ %extend Log { Log(const Log & other) { return new OT::Log(other); } } }
