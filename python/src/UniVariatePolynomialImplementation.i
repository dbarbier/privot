// SWIG file UniVariatePolynomialImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "UniVariatePolynomialImplementation.hxx"
%}

%include UniVariatePolynomialImplementation.hxx
namespace OT { %extend UniVariatePolynomialImplementation { UniVariatePolynomialImplementation(const UniVariatePolynomialImplementation & other) { return new OT::UniVariatePolynomialImplementation(other); } } }
