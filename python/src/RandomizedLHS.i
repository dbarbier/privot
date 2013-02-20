// SWIG file RandomizedLHS.i
// @author dutka
// @date   2011-11-17 13:46:13 +0100 (jeu. 17 nov. 2011)

%{
#include "RandomizedLHS.hxx"
%}

%include RandomizedLHS.hxx
namespace OT { %extend RandomizedLHS { RandomizedLHS(const RandomizedLHS & other) { return new OT::RandomizedLHS(other); } } }
