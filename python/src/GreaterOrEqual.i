// SWIG file GreaterOrEqual.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "GreaterOrEqual.hxx"
%}

%include GreaterOrEqual.hxx
namespace OT {  
%extend GreaterOrEqual {
  const String __str__() const { return self->__repr__(); }
}
}
namespace OT { %extend GreaterOrEqual { GreaterOrEqual(const GreaterOrEqual & other) { return new OT::GreaterOrEqual(other); } } }
