// SWIG file CobylaSpecificParameters.i
// @author dutka
// @date   2009-05-28 14:47:53 +0200 (jeu. 28 mai 2009)

%{
#include "CobylaSpecificParameters.hxx"
%}

%include CobylaSpecificParameters.hxx
namespace OT{ %extend CobylaSpecificParameters { CobylaSpecificParameters(const CobylaSpecificParameters & other) { return new OT::CobylaSpecificParameters(other); } } }
