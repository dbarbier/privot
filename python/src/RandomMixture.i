// SWIG file RandomMixture.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "RandomMixture.hxx"
%}

%include RandomMixture.hxx
namespace OT { %extend RandomMixture { RandomMixture(const RandomMixture & other) { return new OT::RandomMixture(other); } } }
