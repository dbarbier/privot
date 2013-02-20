// SWIG file SecondOrderModelImplementation.i
// @author dutka
// @date   2008-06-26 13:50:17 +0200 (jeu. 26 juin 2008)

%{
#include "SecondOrderModelImplementation.hxx"
%}

%include SecondOrderModelImplementation.hxx
namespace OT{ %extend SecondOrderModelImplementation { SecondOrderModelImplementation(const SecondOrderModelImplementation & other) { return new OT::SecondOrderModelImplementation(other); } } }
