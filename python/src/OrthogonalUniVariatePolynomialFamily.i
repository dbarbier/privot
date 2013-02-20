// SWIG file OrthogonalUniVariatePolynomialFamily.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "OrthogonalUniVariatePolynomialFamily.hxx"

namespace OT { 

  template <>
  struct traitsPythonType<OT::OrthogonalUniVariatePolynomialFamily>
  {
    typedef _PyObject_ Type;
  };


  template <>
  inline
  OT::OrthogonalUniVariatePolynomialFamily
  convert<_PyObject_,OT::OrthogonalUniVariatePolynomialFamily>(PyObject * pyObj)
  {
    void * ptr = 0;
    if (! SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIG_TypeQuery("OT::OrthogonalUniVariatePolynomialFactory *"), 0 |  0 )))
      throw OT::InvalidArgumentException(HERE) << "Object passed as argument is not an object convertible to an OrthogonalUniVariatePolynomialFactory";
    OT::OrthogonalUniVariatePolynomialFactory * p_fact = reinterpret_cast< OT::OrthogonalUniVariatePolynomialFactory * >( ptr );
    return *p_fact;
  }

} /* namespace OT */


%}

%template(OrthogonalUniVariatePolynomialFactoryTypedInterfaceObject)           OT::TypedInterfaceObject<OT::OrthogonalUniVariatePolynomialFactory>;

%include OrthogonalUniVariatePolynomialFamily.hxx

namespace OT{  

%extend OrthogonalUniVariatePolynomialFamily {

OrthogonalUniVariatePolynomialFamily(const OrthogonalUniVariatePolynomialFamily & other) { return new OT::OrthogonalUniVariatePolynomialFamily(other); }

}

}
