// SWIG file Drawable.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Drawable.hxx"
%}

OTDefaultCollectionConvertFunctions(Drawable)

OTTypedInterfaceObjectHelper(Drawable)
OTTypedCollectionInterfaceObjectHelper(Drawable)

%include Drawable.hxx

namespace OT{  

%extend Drawable {

Drawable(const Drawable & other) { return new OT::Drawable(other); }

Drawable(PyObject * pyObj)
{
 return new OT::Drawable( OT::convert<OT::_PyObject_,OT::Drawable>(pyObj) );
}

}
}
