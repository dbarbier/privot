// SWIG file BoundConstrainedAlgorithmImplementationResult.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "BoundConstrainedAlgorithmImplementationResult.hxx"
%}

%include BoundConstrainedAlgorithmImplementationResult.hxx
namespace OT{ %extend BoundConstrainedAlgorithmImplementationResult { BoundConstrainedAlgorithmImplementationResult(const BoundConstrainedAlgorithmImplementationResult & other) { return new OT::BoundConstrainedAlgorithmImplementationResult(other); } } }
