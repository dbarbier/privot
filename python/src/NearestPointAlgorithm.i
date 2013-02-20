// SWIG file NearestPointAlgorithm.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NearestPointAlgorithm.hxx"
%}

OTTypedInterfaceObjectHelper(NearestPointAlgorithm)

%include NearestPointAlgorithm.hxx

namespace OT{ %extend NearestPointAlgorithm { NearestPointAlgorithm(const NearestPointAlgorithm & other) { return new OT::NearestPointAlgorithm(other); } } }
