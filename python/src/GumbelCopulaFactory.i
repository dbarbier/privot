// SWIG file GumbelCopulaFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "GumbelCopulaFactory.hxx"
%}

%include GumbelCopulaFactory.hxx
namespace OT { %extend GumbelCopulaFactory { GumbelCopulaFactory(const GumbelCopulaFactory & other) { return new OT::GumbelCopulaFactory(other); } } }
