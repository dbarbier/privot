// SWIG file FORM.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "FORM.hxx"
%}

%include FORM.hxx
namespace OT{ %extend FORM { FORM(const FORM & other) { return new OT::FORM(other); } } }
