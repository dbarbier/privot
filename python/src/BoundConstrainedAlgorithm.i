// SWIG file BoundConstrainedAlgorithm.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "BoundConstrainedAlgorithm.hxx"
%}

OTTypedInterfaceObjectHelper(BoundConstrainedAlgorithm)

%include BoundConstrainedAlgorithm.hxx

namespace OT{ %extend BoundConstrainedAlgorithm { BoundConstrainedAlgorithm(const BoundConstrainedAlgorithm & other) { return new OT::BoundConstrainedAlgorithm(other); } } }
