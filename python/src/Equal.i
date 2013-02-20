// SWIG file Equal.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Equal.hxx"
%}

%include Equal.hxx
namespace OT {  
%extend Equal {
  const String __str__() const { return self->__repr__(); }
}
}
namespace OT { %extend Equal { Equal(const Equal & other) { return new OT::Equal(other); } } }
