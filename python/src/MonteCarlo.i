// SWIG file MonteCarlo.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "MonteCarlo.hxx"
%}

%include MonteCarlo.hxx
namespace OT{ %extend MonteCarlo { MonteCarlo(const MonteCarlo & other) { return new OT::MonteCarlo(other); } } }
