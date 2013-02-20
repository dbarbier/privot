// SWIG file ConditionalRandomVector.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ConditionalRandomVector.hxx"
%}

%include ConditionalRandomVector.hxx
namespace OT { %extend ConditionalRandomVector { ConditionalRandomVector(const ConditionalRandomVector & other) { return new OT::ConditionalRandomVector(other); } } }
