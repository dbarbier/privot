// SWIG file BoundConstrainedAlgorithmImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "BoundConstrainedAlgorithmImplementation.hxx"
%}

%include BoundConstrainedAlgorithmImplementation.hxx
namespace OT{ %extend BoundConstrainedAlgorithmImplementation { BoundConstrainedAlgorithmImplementation(const BoundConstrainedAlgorithmImplementation & other) { return new OT::BoundConstrainedAlgorithmImplementation(other); } } }
