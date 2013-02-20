// SWIG file ProductPolynomialEvaluationImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ProductPolynomialEvaluationImplementation.hxx"
%}

%template(PolynomialCollection) OT::Collection<OT::UniVariatePolynomial>;

%template(PolynomialPersistentCollection) OT::PersistentCollection<OT::UniVariatePolynomial>;

%include ProductPolynomialEvaluationImplementation.hxx
namespace OT { %extend ProductPolynomialEvaluationImplementation { ProductPolynomialEvaluationImplementation(const ProductPolynomialEvaluationImplementation & other) { return new OT::ProductPolynomialEvaluationImplementation(other); } } }
