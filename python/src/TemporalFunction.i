// SWIG file TemporalFunction.i
// @author dutka
// @date   2010-11-09 13:44:00 +0100 (mar. 09 nov. 2010)

%{
#include "TemporalFunction.hxx"
%}

%include TemporalFunction.hxx
namespace OT { %extend TemporalFunction { TemporalFunction(const TemporalFunction & other) { return new OT::TemporalFunction(other); } } }
