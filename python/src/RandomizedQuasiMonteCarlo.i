// SWIG file RandomizedQuasiMonteCarlo.i
// @author dutka
// @date   2011-11-17 13:46:13 +0100 (jeu. 17 nov. 2011)

%{
#include "RandomizedQuasiMonteCarlo.hxx"
%}

%include RandomizedQuasiMonteCarlo.hxx
namespace OT { %extend RandomizedQuasiMonteCarlo { RandomizedQuasiMonteCarlo(const RandomizedQuasiMonteCarlo & other) { return new OT::RandomizedQuasiMonteCarlo(other); } } }
