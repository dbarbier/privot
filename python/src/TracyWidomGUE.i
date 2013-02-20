// SWIG file TracyWidomGUE.i
// @author dutka
// @date   2008-06-26 13:50:17 +0200 (jeu. 26 juin 2008)

%{
#include "TracyWidomGUE.hxx"
%}

%include TracyWidomGUE.hxx
namespace OT { %extend TracyWidomGUE { TracyWidomGUE(const TracyWidomGUE & other) { return new OT::TracyWidomGUE(other); } } }
