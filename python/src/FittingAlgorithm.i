// SWIG file FittingAlgorithm.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "FittingAlgorithm.hxx"
%}

OTTypedInterfaceObjectHelper(FittingAlgorithm)

%include FittingAlgorithm.hxx
namespace OT { %extend FittingAlgorithm { FittingAlgorithm(const FittingAlgorithm & other) { return new OT::FittingAlgorithm(other); } } }
