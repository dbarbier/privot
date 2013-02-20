// SWIG file BernoulliFactory.i
// @author dutka
// @date   2008-06-26 13:50:17 +0200 (jeu. 26 juin 2008)

%{
#include "BernoulliFactory.hxx"
%}

%include BernoulliFactory.hxx
namespace OT { %extend BernoulliFactory { BernoulliFactory(const BernoulliFactory & other) { return new OT::BernoulliFactory(other); } } }
