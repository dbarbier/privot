// SWIG file SpectralNormalProcess.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "SpectralNormalProcess.hxx"
%}


%include SpectralNormalProcess.hxx
namespace OT { %extend SpectralNormalProcess { SpectralNormalProcess(const SpectralNormalProcess & other) { return new OT::SpectralNormalProcess(other); } } }
