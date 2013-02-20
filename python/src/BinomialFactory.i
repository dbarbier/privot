// SWIG file BinomialFactory.i
// @author dutka
// @date   2008-06-26 13:50:17 +0200 (jeu. 26 juin 2008)

%{
#include "BinomialFactory.hxx"
%}

%include BinomialFactory.hxx
namespace OT { %extend BinomialFactory { BinomialFactory(const BinomialFactory & other) { return new OT::BinomialFactory(other); } } }
