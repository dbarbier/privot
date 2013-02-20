// SWIG file AbdoRackwitz.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "AbdoRackwitz.hxx"
%}

%include AbdoRackwitz.hxx

namespace OT{ %extend AbdoRackwitz { AbdoRackwitz(const AbdoRackwitz & other) { return new OT::AbdoRackwitz(other); } } }
