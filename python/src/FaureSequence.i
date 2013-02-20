// SWIG file FaureSequence.i
// @author dutka
// @date   2009-11-09 17:38:15 +0100 (lun. 09 nov. 2009)

%{
#include "FaureSequence.hxx"
%}

%include FaureSequence.hxx
namespace OT{ %extend FaureSequence { FaureSequence(const FaureSequence & other) { return new OT::FaureSequence(other); } } }
