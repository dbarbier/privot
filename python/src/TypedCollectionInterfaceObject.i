// SWIG file TypedCollectionInterfaceObject.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "TypedCollectionInterfaceObject.hxx"
%}

%include TypedCollectionInterfaceObject.hxx

namespace OT {  
%extend TypedCollectionInterfaceObject {
  void erase(UnsignedLong position) { self->erase(position); }
}
}
