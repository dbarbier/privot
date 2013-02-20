// SWIG file RiskyAndFast.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "RiskyAndFast.hxx"
%}

%include RiskyAndFast.hxx
namespace OT{ %extend RiskyAndFast { RiskyAndFast(const RiskyAndFast & other) { return new OT::RiskyAndFast(other); } } }
