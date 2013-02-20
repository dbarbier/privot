// SWIG file AbdoRackwitzSpecificParameters.i
// @author dutka
// @date   2009-05-28 14:47:53 +0200 (jeu. 28 mai 2009)

%{
#include "AbdoRackwitzSpecificParameters.hxx"
%}

%include AbdoRackwitzSpecificParameters.hxx

namespace OT{ %extend AbdoRackwitzSpecificParameters { AbdoRackwitzSpecificParameters(const AbdoRackwitzSpecificParameters & other) { return new OT::AbdoRackwitzSpecificParameters(other); } } }
