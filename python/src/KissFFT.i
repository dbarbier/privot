// SWIG file KissFFT.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "KissFFT.hxx"
%}

%include KissFFT.hxx
namespace OT { %extend KissFFT { KissFFT(const KissFFT & other) { return new OT::KissFFT(other); } } }
