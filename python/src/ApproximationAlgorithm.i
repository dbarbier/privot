// SWIG file ApproximationAlgorithm.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "ApproximationAlgorithm.hxx"
%}

OTTypedInterfaceObjectHelper(ApproximationAlgorithm)

%include ApproximationAlgorithm.hxx
namespace OT { %extend ApproximationAlgorithm { ApproximationAlgorithm(const ApproximationAlgorithm & other) { return new OT::ApproximationAlgorithm(other); } } }
