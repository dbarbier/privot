// SWIG file StandardDistributionPolynomialFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "StandardDistributionPolynomialFactory.hxx"
%}

%include StandardDistributionPolynomialFactory.hxx
namespace OT { %extend StandardDistributionPolynomialFactory { StandardDistributionPolynomialFactory(const StandardDistributionPolynomialFactory & other) { return new OT::StandardDistributionPolynomialFactory(other); } } }
