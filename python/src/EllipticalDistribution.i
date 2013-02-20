// SWIG file EllipticalDistribution.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "EllipticalDistribution.hxx"
%}

%include EllipticalDistribution.hxx
namespace OT { %extend EllipticalDistribution { EllipticalDistribution(const EllipticalDistribution & other) { return new OT::EllipticalDistribution(other); } } }
