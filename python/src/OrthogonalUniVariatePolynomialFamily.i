// SWIG file OrthogonalUniVariatePolynomialFamily.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "OrthogonalUniVariatePolynomialFamily.hxx"
%}

OTDefaultCollectionConvertFunctionsMisnamed(OrthogonalUniVariatePolynomialFamily, OrthogonalUniVariatePolynomialFactory)

OTTypedInterfaceObjectImplementationHelper(OrthogonalUniVariatePolynomialFamily, OrthogonalUniVariatePolynomialFactory)
OTTypedCollectionInterfaceObjectMisnamedHelper(OrthogonalUniVariatePolynomialFamily, PolynomialFamilyCollection)

%include OrthogonalUniVariatePolynomialFamily.hxx

namespace OT{  

%extend OrthogonalUniVariatePolynomialFamily {

OrthogonalUniVariatePolynomialFamily(const OrthogonalUniVariatePolynomialFamily & other) { return new OT::OrthogonalUniVariatePolynomialFamily(other); }

}

}
