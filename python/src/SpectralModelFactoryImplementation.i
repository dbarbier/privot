// SWIG file SpectralModelFactoryImplementation.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "SpectralModelFactoryImplementation.hxx"
%}

%include SpectralModelFactoryImplementation.hxx
namespace OT {%extend SpectralModelFactoryImplementation {SpectralModelFactoryImplementation(const SpectralModelFactoryImplementation & other){return new OT::SpectralModelFactoryImplementation(other);}}}
