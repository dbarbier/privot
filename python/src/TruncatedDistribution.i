// SWIG file TruncatedDistribution.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "TruncatedDistribution.hxx"
%}

%include TruncatedDistribution.hxx
namespace OT { %extend TruncatedDistribution { TruncatedDistribution(const TruncatedDistribution & other) { return new OT::TruncatedDistribution(other); } } }
