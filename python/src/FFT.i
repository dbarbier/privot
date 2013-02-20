// SWIG file FFT.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "FFT.hxx"
%}

OTTypedInterfaceObjectHelper(FFT)

%include FFT.hxx
namespace OT{ %extend FFT { FFT(const FFT & other) { return new OT::FFT(other); } } }
