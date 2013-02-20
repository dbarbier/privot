// SWIG file OrthogonalProductPolynomialFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "OrthogonalProductPolynomialFactory.hxx"
%}

%template(PolynomialFamilyCollection) OT::Collection<OT::OrthogonalUniVariatePolynomialFamily>;

%template(PolynomialFamilyPersistentCollection) OT::PersistentCollection<OT::OrthogonalUniVariatePolynomialFamily>;

%include OrthogonalProductPolynomialFactory.hxx
namespace OT{ %extend OrthogonalProductPolynomialFactory { OrthogonalProductPolynomialFactory(const OrthogonalProductPolynomialFactory & other) { return new OT::OrthogonalProductPolynomialFactory(other); } } }
