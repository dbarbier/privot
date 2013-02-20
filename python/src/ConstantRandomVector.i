// SWIG file ConstantRandomVector.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ConstantRandomVector.hxx"
%}

%include ConstantRandomVector.hxx
namespace OT { %extend ConstantRandomVector { ConstantRandomVector(const ConstantRandomVector & other) { return new OT::ConstantRandomVector(other); } } }
