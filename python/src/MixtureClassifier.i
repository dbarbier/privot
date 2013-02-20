// SWIG file MixtureClassifier.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "MixtureClassifier.hxx"
%}

%include MixtureClassifier.hxx
namespace OT { %extend MixtureClassifier { MixtureClassifier(const MixtureClassifier & other) { return new OT::MixtureClassifier(other); } } }
