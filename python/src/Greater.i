// SWIG file Greater.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Greater.hxx"
%}

%include Greater.hxx
namespace OT {  
%extend Greater {
  const String __str__() const { return self->__repr__(); }
}
}
namespace OT { %extend Greater { Greater(const Greater & other) { return new OT::Greater(other); } } }
