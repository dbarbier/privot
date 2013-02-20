// SWIG file NormalFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NormalFactory.hxx"
%}

%include NormalFactory.hxx
namespace OT { %extend NormalFactory { NormalFactory(const NormalFactory & other) { return new OT::NormalFactory(other); } } }
