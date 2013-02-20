// SWIG file ApproximationAlgorithmImplementation.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "ApproximationAlgorithmImplementation.hxx"
%}

%include ApproximationAlgorithmImplementation.hxx
namespace OT { %extend ApproximationAlgorithmImplementation { ApproximationAlgorithmImplementation(const ApproximationAlgorithmImplementation & other) { return new OT::ApproximationAlgorithmImplementation(other); } } }
