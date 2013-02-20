// SWIG file LessOrEqual.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LessOrEqual.hxx"
%}

%include LessOrEqual.hxx
namespace OT {  
%extend LessOrEqual {
  const String __str__() const { return self->__repr__(); }
}
}
namespace OT { %extend LessOrEqual { LessOrEqual(const LessOrEqual & other) { return new OT::LessOrEqual(other); } } }
