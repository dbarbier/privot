// SWIG file DirichletFactory.i
// @author dutka
// @date   2008-06-26 13:50:17 +0200 (jeu. 26 juin 2008)

%{
#include "DirichletFactory.hxx"
%}

%include DirichletFactory.hxx
namespace OT { %extend DirichletFactory { DirichletFactory(const DirichletFactory & other) { return new OT::DirichletFactory(other); } } }
