// SWIG file NonCentralChiSquare.i
// @author dutka
// @date   2008-06-26 13:50:17 +0200 (jeu. 26 juin 2008)

%{
#include "NonCentralChiSquare.hxx"
%}

%include NonCentralChiSquare.hxx
namespace OT { %extend NonCentralChiSquare { NonCentralChiSquare(const NonCentralChiSquare & other) { return new OT::NonCentralChiSquare(other); } } }
