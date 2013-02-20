// SWIG file Burr.i
// @author dutka
// @date   2009-11-09 17:38:15 +0100 (lun. 09 nov. 2009)

%{
#include "Burr.hxx"
%}

%include Burr.hxx
namespace OT { %extend Burr { Burr(const Burr & other) { return new OT::Burr(other); } } }
