// SWIG file FunctionalChaosRandomVector.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "FunctionalChaosRandomVector.hxx"
%}

%include FunctionalChaosRandomVector.hxx
namespace OT { %extend FunctionalChaosRandomVector { FunctionalChaosRandomVector(const FunctionalChaosRandomVector & other) { return new OT::FunctionalChaosRandomVector(other); } } }
