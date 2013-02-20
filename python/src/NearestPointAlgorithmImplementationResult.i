// SWIG file NearestPointAlgorithmImplementationResult.i
// @author dutka
// @date   2009-05-28 14:47:53 +0200 (jeu. 28 mai 2009)

%{
#include "NearestPointAlgorithmImplementationResult.hxx"
%}

%include NearestPointAlgorithmImplementationResult.hxx
namespace OT{ %extend NearestPointAlgorithmImplementationResult { NearestPointAlgorithmImplementationResult(const NearestPointAlgorithmImplementationResult & other) { return new OT::NearestPointAlgorithmImplementationResult(other); } } }
