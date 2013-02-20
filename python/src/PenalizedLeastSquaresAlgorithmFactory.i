// SWIG file PenalizedLeastSquaresAlgorithmFactory.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "PenalizedLeastSquaresAlgorithmFactory.hxx"
%}

%include PenalizedLeastSquaresAlgorithmFactory.hxx
namespace OT { %extend PenalizedLeastSquaresAlgorithmFactory { PenalizedLeastSquaresAlgorithmFactory(const PenalizedLeastSquaresAlgorithmFactory & other) { return new OT::PenalizedLeastSquaresAlgorithmFactory(other); } } }
