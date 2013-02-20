// SWIG file KernelMixture.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "KernelMixture.hxx"
%}

%include KernelMixture.hxx
namespace OT { %extend KernelMixture { KernelMixture(const KernelMixture & other) { return new OT::KernelMixture(other); } } }
namespace OT { %extend KernelMixture { KernelMixture(const OT::DistributionImplementation & kernel, const OT::NumericalPoint & bandwidth, const OT::NumericalSample & sample) { return new OT::KernelMixture(OT::Distribution(kernel), bandwidth, sample); } } }
