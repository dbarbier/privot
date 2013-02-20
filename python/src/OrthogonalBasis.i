// SWIG file OrthogonalBasis.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "OrthogonalBasis.hxx"
%}

%template(OrthogonalFunctionFactoryTypedInterfaceObject)           OT::TypedInterfaceObject<OT::OrthogonalFunctionFactory>;

%include OrthogonalBasis.hxx

namespace OT{ %extend OrthogonalBasis { OrthogonalBasis(const OrthogonalBasis & other) { return new OT::OrthogonalBasis(other); } } }
