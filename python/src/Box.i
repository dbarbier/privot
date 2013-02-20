// SWIG file Box.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Box.hxx"
%}

%include Box.hxx
namespace OT { %extend Box { Box(const Box & other) { return new OT::Box(other); } } }
