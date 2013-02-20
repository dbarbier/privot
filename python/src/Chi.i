// SWIG file Chi.i
// @author dutka
// @date   2009-11-09 17:38:15 +0100 (lun. 09 nov. 2009)

%{
#include "Chi.hxx"
%}

%include Chi.hxx
namespace OT { %extend Chi { Chi(const Chi & other) { return new OT::Chi(other); } } }
