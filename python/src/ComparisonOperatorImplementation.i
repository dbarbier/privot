// SWIG file ComparisonOperatorImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ComparisonOperatorImplementation.hxx"
%}

%include ComparisonOperatorImplementation.hxx
namespace OT { %extend ComparisonOperatorImplementation { ComparisonOperatorImplementation(const ComparisonOperatorImplementation & other) { return new OT::ComparisonOperatorImplementation(other); } } }
