// SWIG file UsualRandomVector.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "UsualRandomVector.hxx"
%}

%include UsualRandomVector.hxx
namespace OT { %extend UsualRandomVector { UsualRandomVector(const UsualRandomVector & other) { return new OT::UsualRandomVector(other); } } }
