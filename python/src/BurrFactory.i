// SWIG file BurrFactory.i
// @author dutka
// @date   2008-06-26 13:50:17 +0200 (jeu. 26 juin 2008)

%{
#include "BurrFactory.hxx"
%}

%include BurrFactory.hxx
namespace OT { %extend BurrFactory { BurrFactory(const BurrFactory & other) { return new OT::BurrFactory(other); } } }
