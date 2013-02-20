// SWIG file ConditionalDistribution.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (lun. 02 janv. 2012)

%{
#include "ConditionalDistribution.hxx"
%}

%include ConditionalDistribution.hxx
namespace OT { %extend ConditionalDistribution { ConditionalDistribution(const ConditionalDistribution & other) { return new OT::ConditionalDistribution(other); } } }
