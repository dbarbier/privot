// SWIG file Dirichlet.i
// @author dutka
// @date   2007-03-07 15:50:39 +0100 (mer. 07 mars 2007)

%{
#include "Dirichlet.hxx"
%}

%include Dirichlet.hxx
namespace OT { %extend Dirichlet { Dirichlet(const Dirichlet & other) { return new OT::Dirichlet(other); } } }
