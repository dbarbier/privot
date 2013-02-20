// SWIG file Student.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Student.hxx"
%}

%include Student.hxx
namespace OT { %extend Student { Student(const Student & other) { return new OT::Student(other); } } }
