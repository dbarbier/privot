// SWIG file QuasiMonteCarlo.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "QuasiMonteCarlo.hxx"
%}

%include QuasiMonteCarlo.hxx
namespace OT{ %extend QuasiMonteCarlo { QuasiMonteCarlo(const QuasiMonteCarlo & other) { return new OT::QuasiMonteCarlo(other); } } }
