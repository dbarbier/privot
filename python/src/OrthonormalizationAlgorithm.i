// SWIG file OrthonormalizationAlgorithm.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "OrthonormalizationAlgorithm.hxx"
%}

OTTypedInterfaceObjectHelper(OrthonormalizationAlgorithm)

%include OrthonormalizationAlgorithm.hxx
namespace OT{ %extend OrthonormalizationAlgorithm { OrthonormalizationAlgorithm(const OrthonormalizationAlgorithm & other) { return new OT::OrthonormalizationAlgorithm(other); } } }
