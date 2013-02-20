// SWIG file StudentFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "StudentFactory.hxx"
%}

%include StudentFactory.hxx
namespace OT { %extend StudentFactory { StudentFactory(const StudentFactory & other) { return new OT::StudentFactory(other); } } }
