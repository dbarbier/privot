// SWIG file Bernoulli.i
// @author dutka
// @date   2007-03-07 15:50:39 +0100 (mer. 07 mars 2007)

%{
#include "Bernoulli.hxx"
%}

%include Bernoulli.hxx
namespace OT { %extend Bernoulli { Bernoulli(const Bernoulli & other) { return new OT::Bernoulli(other); } } }
