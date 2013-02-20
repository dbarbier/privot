// SWIG file LowDiscrepancySequenceImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LowDiscrepancySequenceImplementation.hxx"
%}

%include LowDiscrepancySequenceImplementation.hxx
namespace OT{ %extend LowDiscrepancySequenceImplementation { LowDiscrepancySequenceImplementation(const LowDiscrepancySequenceImplementation & other) { return new OT::LowDiscrepancySequenceImplementation(other); } } }
