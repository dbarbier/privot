// SWIG file Beta.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Beta.hxx"
%}

%include Beta.hxx
namespace OT { %extend Beta { Beta(const Beta & other) { return new OT::Beta(other); } } }
