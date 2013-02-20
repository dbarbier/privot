// SWIG file ReverseHaltonSequence.i
// @author dutka
// @date   2009-11-09 17:38:15 +0100 (lun. 09 nov. 2009)

%{
#include "ReverseHaltonSequence.hxx"
%}

%include ReverseHaltonSequence.hxx
namespace OT{ %extend ReverseHaltonSequence { ReverseHaltonSequence(const ReverseHaltonSequence & other) { return new OT::ReverseHaltonSequence(other); } } }
