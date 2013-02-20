// SWIG file OrthogonalUniVariatePolynomial.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "OrthogonalUniVariatePolynomial.hxx"
%}

%include OrthogonalUniVariatePolynomial.hxx
namespace OT { %extend OrthogonalUniVariatePolynomial { OrthogonalUniVariatePolynomial(const OrthogonalUniVariatePolynomial & other) { return new OT::OrthogonalUniVariatePolynomial(other); } } }
