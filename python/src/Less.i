// SWIG file Less.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Less.hxx"
%}

%include Less.hxx
namespace OT {  
%extend Less {
  const String __str__() const { return self->__repr__(); }
}
}
namespace OT { %extend Less { Less(const Less & other) { return new OT::Less(other); } } }
