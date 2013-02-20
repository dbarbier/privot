// SWIG file SpectralModelImplementation.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "SpectralModelImplementation.hxx"
%}


%include SpectralModelImplementation.hxx
namespace OT{ %extend SpectralModelImplementation { SpectralModelImplementation(const SpectralModelImplementation & other) { return new OT::SpectralModelImplementation(other); } } }
