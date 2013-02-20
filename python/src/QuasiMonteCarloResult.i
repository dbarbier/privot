// SWIG file QuasiMonteCarloResult.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "QuasiMonteCarloResult.hxx"
%}

%include QuasiMonteCarloResult.hxx
namespace OT{ %extend QuasiMonteCarloResult { QuasiMonteCarloResult(const QuasiMonteCarloResult & other) { return new OT::QuasiMonteCarloResult(other); } } }
