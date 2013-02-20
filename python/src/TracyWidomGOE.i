// SWIG file TracyWidomGOE.i
// @author dutka
// @date   2008-06-26 13:50:17 +0200 (jeu. 26 juin 2008)

%{
#include "TracyWidomGOE.hxx"
%}

%include TracyWidomGOE.hxx
namespace OT { %extend TracyWidomGOE { TracyWidomGOE(const TracyWidomGOE & other) { return new OT::TracyWidomGOE(other); } } }
