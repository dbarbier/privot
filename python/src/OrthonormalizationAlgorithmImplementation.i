// SWIG file OrthonormalizationAlgorithmImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "OrthonormalizationAlgorithmImplementation.hxx"
%}

%include OrthonormalizationAlgorithmImplementation.hxx
namespace OT{ %extend OrthonormalizationAlgorithmImplementation { OrthonormalizationAlgorithmImplementation(const OrthonormalizationAlgorithmImplementation & other) { return new OT::OrthonormalizationAlgorithmImplementation(other); } } }
