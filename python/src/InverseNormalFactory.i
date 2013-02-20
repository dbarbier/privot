// SWIG file InverseNormalFactory.i
// @author dutka
// @date   2008-06-26 13:50:17 +0200 (jeu. 26 juin 2008)

%{
#include "InverseNormalFactory.hxx"
%}

%include InverseNormalFactory.hxx
namespace OT { %extend InverseNormalFactory { InverseNormalFactory(const InverseNormalFactory & other) { return new OT::InverseNormalFactory(other); } } }
