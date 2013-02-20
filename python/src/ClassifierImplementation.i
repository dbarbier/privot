// SWIG file ClassifierImplementation.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "ClassifierImplementation.hxx"
%}

%include ClassifierImplementation.hxx
namespace OT { %extend ClassifierImplementation { ClassifierImplementation(const ClassifierImplementation & other) { return new OT::ClassifierImplementation(other); } } }
