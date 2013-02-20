// SWIG file PenalizedLeastSquaresAlgorithm.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "PenalizedLeastSquaresAlgorithm.hxx"
%}

%include PenalizedLeastSquaresAlgorithm.hxx
namespace OT{ %extend PenalizedLeastSquaresAlgorithm { PenalizedLeastSquaresAlgorithm(const PenalizedLeastSquaresAlgorithm & other) { return new OT::PenalizedLeastSquaresAlgorithm(other); } } }
