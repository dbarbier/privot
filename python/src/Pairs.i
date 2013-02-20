// SWIG file Triangular.i
// @author dutka
// @date   2007-03-07 15:50:39 +0100 (mer. 07 mars 2007)

%{
#include "Pairs.hxx"
%}

%include Pairs.hxx

namespace OT { %extend Pairs { Pairs(const Pairs & other) { return new OT::Pairs(other); } } }  

