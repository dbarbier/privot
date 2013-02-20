// SWIG file GumbelCopula.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "GumbelCopula.hxx"
%}

%include GumbelCopula.hxx
namespace OT { %extend GumbelCopula { GumbelCopula(const GumbelCopula & other) { return new OT::GumbelCopula(other); } } }
