// SWIG file MinCopula.i
// @author dutka
// @date   2011-11-17 13:46:13 +0100 (jeu. 17 nov. 2011)

%{
#include "MinCopula.hxx"
%}

%include MinCopula.hxx
namespace OT { %extend MinCopula { MinCopula(const MinCopula & other) { return new OT::MinCopula(other); } } }
