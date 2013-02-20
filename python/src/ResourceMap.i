// SWIG file ResourceMap.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ResourceMap.hxx"
%}

%template(StringStringMap) std::map<OT::String,OT::String>;

%include ResourceMap.hxx
namespace OT {  
%extend ResourceMap {
  const String __str__() const { return self->__repr__(); }
}
}
