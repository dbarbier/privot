// SWIG file Multinomial.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Multinomial.hxx"
%}

%include Multinomial.hxx
namespace OT { %extend Multinomial { Multinomial(const Multinomial & other) { return new OT::Multinomial(other); } } }
