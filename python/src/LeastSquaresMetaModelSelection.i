// SWIG file LeastSquaresMetaModelSelection.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "LeastSquaresMetaModelSelection.hxx"
%}

%include LeastSquaresMetaModelSelection.hxx
namespace OT { %extend LeastSquaresMetaModelSelection { LeastSquaresMetaModelSelection(const LeastSquaresMetaModelSelection & other) { return new OT::LeastSquaresMetaModelSelection(other); } } }
