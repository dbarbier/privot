// SWIG file TypedInterfaceObject.i
// @author dutka
// @date   2008-06-26 13:50:17 +0200 (Thu, 26 Jun 2008)

%{
#include "TypedInterfaceObject.hxx"
%}


%define OTTypedInterfaceObjectHelper(Interface)

%template(Interface ## ImplementationTypedInterfaceObject)           OT::TypedInterfaceObject<OT::Interface ## Implementation>;

%typemap(in) const OT::Interface & {
  void * ptr = 0;
  if (SWIG_IsOK(SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor, 0))) {
    // From interface class, ok
  } else if (SWIG_IsOK(SWIG_ConvertPtr($input, &ptr, SWIGTYPE_p_OT__ ## Interface ## Implementation, 0))) {
    // From Implementation*
    OT::Interface ## Implementation * p_impl = reinterpret_cast< OT::Interface ## Implementation * >( ptr );
    $1 = new OT::Interface( *p_impl );
  } else if (SWIG_IsOK(SWIG_ConvertPtr($input, &ptr, SWIG_TypeQuery("OT::Pointer<OT::Interface" "Implementation> *"), 0))) {
    // From Pointer<Implementation>
    OT::Pointer<OT::Interface ## Implementation> * p_impl = reinterpret_cast< OT::Pointer<OT::Interface ## Implementation> * >( ptr );
    $1 = new OT::Interface( **p_impl );
  }
}

%typemap(typecheck,precedence=SWIG_TYPECHECK_POINTER) const OT::Interface & {
  $1 = SWIG_IsOK(SWIG_ConvertPtr($input, NULL, $1_descriptor, 0))
    || SWIG_IsOK(SWIG_ConvertPtr($input, NULL, SWIGTYPE_p_OT__ ## Interface ## Implementation, 0))
    || SWIG_IsOK(SWIG_ConvertPtr($input, NULL, SWIG_TypeQuery("OT::Pointer<OT::Interface" "Implementation> *"), 0));
}

%enddef


%include TypedInterfaceObject.hxx
