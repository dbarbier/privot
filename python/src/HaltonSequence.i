// SWIG file HaltonSequence.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "HaltonSequence.hxx"
%}

%include HaltonSequence.hxx
namespace OT{ %extend HaltonSequence { HaltonSequence(const HaltonSequence & other) { return new OT::HaltonSequence(other); } } }
