// SWIG file ARMA.i
// @author souchaud
// @date   2011-04-05 17:50:07 +0200 (mar. 05 avril 2011)

%{
#include "ARMA.hxx"
%}

%include ARMA.hxx
namespace OT { %extend ARMA { ARMA(const ARMA & other) { return new OT::ARMA(other); } } }
