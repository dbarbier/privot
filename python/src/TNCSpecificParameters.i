// SWIG file TNCSpecificParameters.i
// @author dutka
// @date   2009-05-28 14:47:53 +0200 (jeu. 28 mai 2009)

%{
#include "TNCSpecificParameters.hxx"
%}

%include TNCSpecificParameters.hxx
namespace OT{ %extend TNCSpecificParameters { TNCSpecificParameters(const TNCSpecificParameters & other) { return new OT::TNCSpecificParameters(other); } } }
