// SWIG file ExponentialCauchy.i
// @author dutka
// @date   2008-06-26 13:50:17 +0200 (jeu. 26 juin 2008)

%{
#include "ExponentialCauchy.hxx"
%}

%include ExponentialCauchy.hxx
namespace OT { %extend ExponentialCauchy { ExponentialCauchy(const ExponentialCauchy & other) { return new OT::ExponentialCauchy(other); } } }
