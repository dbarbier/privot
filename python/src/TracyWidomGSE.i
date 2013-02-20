// SWIG file TracyWidomGSE.i
// @author dutka
// @date   2008-06-26 13:50:17 +0200 (jeu. 26 juin 2008)

%{
#include "TracyWidomGSE.hxx"
%}

%include TracyWidomGSE.hxx
namespace OT { %extend TracyWidomGSE { TracyWidomGSE(const TracyWidomGSE & other) { return new OT::TracyWidomGSE(other); } } }
