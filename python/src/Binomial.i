// SWIG file Binomial.i
// @author dutka
// @date   2007-03-07 15:50:39 +0100 (mer. 07 mars 2007)

%{
#include "Binomial.hxx"
%}

%include Binomial.hxx
namespace OT { %extend Binomial { Binomial(const Binomial & other) { return new OT::Binomial(other); } } }
