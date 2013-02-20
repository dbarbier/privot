// SWIG file Dirac.i
// @author dutka
// @date   2007-03-07 15:50:39 +0100 (mer. 07 mars 2007)

%{
#include "Dirac.hxx"
%}

%include Dirac.hxx
namespace OT { %extend Dirac { Dirac(const Dirac & other) { return new OT::Dirac(other); } } }
