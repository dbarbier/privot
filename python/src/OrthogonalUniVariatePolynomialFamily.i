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

%typemap(in) const OT::OrthogonalUniVariatePolynomialFamily & {
  void * ptr = 0;
  if (SWIG_IsOK(SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor, 0)))
  {
   // Nothing to do here
  } else if (SWIG_IsOK(SWIG_ConvertPtr($input, &ptr, SWIGTYPE_p_OT__OrthogonalUniVariatePolynomialFactory, 0))) {
    // From Implementation*
    OT::OrthogonalUniVariatePolynomialFactory * p_impl = reinterpret_cast< OT::OrthogonalUniVariatePolynomialFactory * >( ptr );
    $1 = new OT::OrthogonalUniVariatePolynomialFamily( *p_impl );
  }
}

%typemap(typecheck,precedence=SWIG_TYPECHECK_POINTER) const OT::OrthogonalUniVariatePolynomialFamily & {
  $1 = SWIG_IsOK(SWIG_ConvertPtr($input, NULL, $1_descriptor, 0))
    || SWIG_IsOK(SWIG_ConvertPtr($input, NULL, SWIGTYPE_p_OT__OrthogonalUniVariatePolynomialFactory, 0));
}

namespace OT{  

%extend OrthogonalUniVariatePolynomialFamily {

OrthogonalUniVariatePolynomialFamily(const OrthogonalUniVariatePolynomialFamily & other) { return new OT::OrthogonalUniVariatePolynomialFamily(other); }

}

}
