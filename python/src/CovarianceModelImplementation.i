// SWIG file CovarianceModelImplementation.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "CovarianceModelImplementation.hxx"
%}

%include CovarianceModelImplementation.hxx
namespace OT{ %extend CovarianceModelImplementation { CovarianceModelImplementation(const CovarianceModelImplementation & other) { return new OT::CovarianceModelImplementation(other); } } }
