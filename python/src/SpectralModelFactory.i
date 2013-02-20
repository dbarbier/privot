// SWIG file SpectralModelFactory.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "SpectralModelFactory.hxx"
%}

OTTypedInterfaceObjectHelper(SpectralModelFactory)

%include SpectralModelFactory.hxx

namespace OT { 

  %extend SpectralModelFactory {

     SpectralModelFactory(const SpectralModelFactory & other) { return new OT::SpectralModelFactory(other); }

   }
 }
