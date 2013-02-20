// SWIG file FittingAlgorithmImplementation.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "FittingAlgorithmImplementation.hxx"
%}

%include FittingAlgorithmImplementation.hxx
namespace OT { %extend FittingAlgorithmImplementation { FittingAlgorithmImplementation(const FittingAlgorithmImplementation & other) { return new OT::FittingAlgorithmImplementation(other); } } }
