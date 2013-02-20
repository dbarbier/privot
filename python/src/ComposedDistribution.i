// SWIG file ComposedDistribution.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ComposedDistribution.hxx"
%}

%include ComposedDistribution.hxx
namespace OT { %extend ComposedDistribution { ComposedDistribution(const ComposedDistribution & other) { return new OT::ComposedDistribution(other); } } }
