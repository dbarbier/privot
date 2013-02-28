// SWIG file DistributionFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "DistributionFactory.hxx"

namespace OT { 

  template <>
  struct traitsPythonType< OT::DistributionFactory >
  {
    typedef _PyObject_ Type;
  };

  template <>
  inline
  OT::DistributionFactory
  convert< _PyObject_, OT::DistributionFactory >(PyObject * pyObj)
  {
    void * ptr = 0;
    if ( SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIG_TypeQuery("OT::DistributionImplementationFactory *"), 0 |  0 ) ) ) {
      OT::DistributionImplementationFactory * p_distf = reinterpret_cast< OT::DistributionImplementationFactory * >( ptr );
      return *p_distf;
    }
    else if ( SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIG_TypeQuery("OT::DistributionFactory *"), 0 |  0 ) ) ) {
      OT::DistributionFactory * p_distf = reinterpret_cast< OT::DistributionFactory * >( ptr );
      return *p_distf;
    }
    else {
      throw OT::InvalidArgumentException(HERE) << "Object passed as argument is not convertible to a DistributionFactory";
    }
    return OT::DistributionFactory();
  }


} /* namespace OT */

%}

%template(DistributionImplementationFactoryTypedInterfaceObject)           OT::TypedInterfaceObject<OT::DistributionImplementationFactory>;
%include DistributionFactory.hxx

%typemap(in) const OT::DistributionFactory & {
  void * ptr = 0;
  if (SWIG_IsOK(SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor, 0)))
  {
   // Nothing to do here
  } else if (SWIG_IsOK(SWIG_ConvertPtr($input, &ptr, SWIGTYPE_p_OT__DistributionImplementationFactory, 0))) {
    // From Implementation*
    OT::DistributionImplementationFactory * p_impl = reinterpret_cast< OT::DistributionImplementationFactory * >( ptr );
    $1 = new OT::DistributionFactory( *p_impl );
  }
}

%typemap(typecheck,precedence=SWIG_TYPECHECK_POINTER) const OT::DistributionFactory & {
  $1 = SWIG_IsOK(SWIG_ConvertPtr($input, NULL, $1_descriptor, 0))
    || SWIG_IsOK(SWIG_ConvertPtr($input, NULL, SWIGTYPE_p_OT__DistributionImplementationFactory, 0));
}

namespace OT {  

%extend DistributionFactory {

DistributionFactory(const DistributionFactory & other) { return new OT::DistributionFactory(other); }

}
}
