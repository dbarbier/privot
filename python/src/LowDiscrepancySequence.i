// SWIG file LowDiscrepancySequence.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LowDiscrepancySequence.hxx"
%}

OTTypedInterfaceObjectHelper(LowDiscrepancySequence)

%include LowDiscrepancySequence.hxx
namespace OT{ %extend LowDiscrepancySequence { LowDiscrepancySequence(const LowDiscrepancySequence & other) { return new OT::LowDiscrepancySequence(other); } } }
