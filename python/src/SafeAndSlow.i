// SWIG file SafeAndSlow.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "SafeAndSlow.hxx"
%}

%include SafeAndSlow.hxx
namespace OT{ %extend SafeAndSlow { SafeAndSlow(const SafeAndSlow & other) { return new OT::SafeAndSlow(other); } } }
