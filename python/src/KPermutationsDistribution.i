// SWIG file KPermutationsDistribution.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "KPermutationsDistribution.hxx"
%}

%include KPermutationsDistribution.hxx
namespace OT { %extend KPermutationsDistribution { KPermutationsDistribution(const KPermutationsDistribution & other) { return new OT::KPermutationsDistribution(other); } } }
