// SWIG file OrthogonalUniVariatePolynomialFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "OrthogonalUniVariatePolynomialFactory.hxx"
%}

%include OrthogonalUniVariatePolynomialFactory.hxx
namespace OT{ %extend OrthogonalUniVariatePolynomialFactory { OrthogonalUniVariatePolynomialFactory(const OrthogonalUniVariatePolynomialFactory & other) { return new OT::OrthogonalUniVariatePolynomialFactory(other); } } }
