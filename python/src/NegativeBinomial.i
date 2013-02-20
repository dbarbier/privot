// SWIG file NegativeBinomial.i
// @author dutka
// @date   2007-03-07 15:50:39 +0100 (mer. 07 mars 2007)

%{
#include "NegativeBinomial.hxx"
%}

%include NegativeBinomial.hxx
namespace OT { %extend NegativeBinomial { NegativeBinomial(const NegativeBinomial & other) { return new OT::NegativeBinomial(other); } } }
