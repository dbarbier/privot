// SWIG file NegativeBinomialFactory.i
// @author dutka
// @date   2008-06-26 13:50:17 +0200 (jeu. 26 juin 2008)

%{
#include "NegativeBinomialFactory.hxx"
%}

%include NegativeBinomialFactory.hxx
namespace OT { %extend NegativeBinomialFactory { NegativeBinomialFactory(const NegativeBinomialFactory & other) { return new OT::NegativeBinomialFactory(other); } } }
