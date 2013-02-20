// SWIG file ApproximationAlgorithmImplementationFactory.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "ApproximationAlgorithmImplementationFactory.hxx"
%}

%include ApproximationAlgorithmImplementationFactory.hxx
namespace OT { %extend ApproximationAlgorithmImplementationFactory { ApproximationAlgorithmImplementationFactory(const ApproximationAlgorithmImplementationFactory & other) { return new OT::ApproximationAlgorithmImplementationFactory(other); } } }
