// SWIG file ComparisonOperator.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ComparisonOperator.hxx"
%}

OTTypedInterfaceObjectHelper(ComparisonOperator)

%include ComparisonOperator.hxx
namespace OT {  
%extend ComparisonOperator {
  const String __str__() const { return self->__repr__(); }
}
}
namespace OT { %extend ComparisonOperator { ComparisonOperator(const ComparisonOperator & other) { return new OT::ComparisonOperator(other); } } }
