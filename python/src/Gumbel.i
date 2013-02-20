// SWIG file Gumbel.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Gumbel.hxx"
%}

%include Gumbel.hxx
namespace OT { %extend Gumbel { Gumbel(const Gumbel & other) { return new OT::Gumbel(other); } } }
