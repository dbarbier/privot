// SWIG file PosteriorRandomVector.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "PosteriorRandomVector.hxx"
%}

%include PosteriorRandomVector.hxx
namespace OT { %extend PosteriorRandomVector { PosteriorRandomVector(const PosteriorRandomVector & other) { return new OT::PosteriorRandomVector(other); } } }
