// SWIG file NearestPointAlgorithmImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NearestPointAlgorithmImplementation.hxx"
%}

%include NearestPointAlgorithmImplementation.hxx
namespace OT{ %extend NearestPointAlgorithmImplementation { NearestPointAlgorithmImplementation(const NearestPointAlgorithmImplementation & other) { return new OT::NearestPointAlgorithmImplementation(other); } } }
