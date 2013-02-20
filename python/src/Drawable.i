// SWIG file Drawable.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Drawable.hxx"

namespace OT { 

  template <>
  struct traitsPythonType< OT::Drawable >
  {
    typedef _PyObject_ Type;
  };

  template <>
  inline
  OT::Drawable
  convert< _PyObject_, OT::Drawable >(PyObject * pyObj)
  {
    void * ptr = 0;
    if ( SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIG_TypeQuery("OT::DrawableImplementation *"), 0 |  0 ) ) ) {
      OT::DrawableImplementation * p_dist = reinterpret_cast< OT::DrawableImplementation * >( ptr );
      return *p_dist;
    }
    else if ( SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIG_TypeQuery("OT::Drawable *"), 0 |  0 ) ) ) {
      OT::Drawable * p_dist = reinterpret_cast< OT::Drawable * >( ptr );
      return *p_dist;
    }
    else {
      throw OT::InvalidArgumentException(HERE) << "Object passed as argument is not convertible to a Drawable";  
    }
    return OT::Drawable();
  }


} /* namespace OT */

%}

OTTypedInterfaceObjectHelper(Drawable)

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
