// SWIG file DistributionImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "DistributionImplementation.hxx"
%}

%include DistributionImplementation.hxx
namespace OT { %extend DistributionImplementation { DistributionImplementation(const DistributionImplementation & other) { return new OT::DistributionImplementation(other); } } }
