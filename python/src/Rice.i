// SWIG file Rice.i
// @author dutka
// @date   2009-11-09 17:38:15 +0100 (lun. 09 nov. 2009)

%{
#include "Rice.hxx"
%}

%include Rice.hxx
namespace OT { %extend Rice { Rice(const Rice & other) { return new OT::Rice(other); } } }
