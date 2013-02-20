// SWIG file Lasso.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "Lasso.hxx"
%}

%include Lasso.hxx
namespace OT { %extend Lasso { Lasso(const Lasso & other) { return new OT::Lasso(other); } } }
