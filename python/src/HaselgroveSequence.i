// SWIG file HaselgroveSequence.i
// @author dutka
// @date   2009-11-09 17:38:15 +0100 (lun. 09 nov. 2009)

%{
#include "HaselgroveSequence.hxx"
%}

%include HaselgroveSequence.hxx
namespace OT{ %extend HaselgroveSequence { HaselgroveSequence(const HaselgroveSequence & other) { return new OT::HaselgroveSequence(other); } } }
