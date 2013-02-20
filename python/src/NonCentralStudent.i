// SWIG file NonCentralStudent.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NonCentralStudent.hxx"
%}

%include NonCentralStudent.hxx
namespace OT { %extend NonCentralStudent { NonCentralStudent(const NonCentralStudent & other) { return new OT::NonCentralStudent(other); } } }
