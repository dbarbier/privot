// SWIG file Classifier.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "Classifier.hxx"
%}

OTTypedInterfaceObjectHelper(Classifier)

%include Classifier.hxx
namespace OT { %extend Classifier { Classifier(const Classifier & other) { return new OT::Classifier(other); } } }

