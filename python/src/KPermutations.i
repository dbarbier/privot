// SWIG file KPermutations.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "KPermutations.hxx"
%}

%include KPermutations.hxx
namespace OT { %extend KPermutations { KPermutations(const KPermutations & other) { return new OT::KPermutations(other); } } }
