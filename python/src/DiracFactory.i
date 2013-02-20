// SWIG file DiracFactory.i
// @author dutka
// @date   2008-06-26 13:50:17 +0200 (jeu. 26 juin 2008)

%{
#include "DiracFactory.hxx"
%}

%include DiracFactory.hxx
namespace OT { %extend DiracFactory { DiracFactory(const DiracFactory & other) { return new OT::DiracFactory(other); } } }
