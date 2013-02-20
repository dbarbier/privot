// SWIG file SpectralModel.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "SpectralModel.hxx"
%}

OTTypedInterfaceObjectHelper(SpectralModel)

%include SpectralModel.hxx
namespace OT{ %extend SpectralModel { SpectralModel(const SpectralModel & other) { return new OT::SpectralModel(other); } } }
