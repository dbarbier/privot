// SWIG file UserDefinedFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "UserDefinedFactory.hxx"
%}

%include UserDefinedFactory.hxx
namespace OT { %extend UserDefinedFactory { UserDefinedFactory(const UserDefinedFactory & other) { return new OT::UserDefinedFactory(other); } } }
