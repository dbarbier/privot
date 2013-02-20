// SWIG file CompositeRandomVector.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "CompositeRandomVector.hxx"
%}

%include CompositeRandomVector.hxx
namespace OT { %extend CompositeRandomVector { CompositeRandomVector(const CompositeRandomVector & other) { return new OT::CompositeRandomVector(other); } } }
