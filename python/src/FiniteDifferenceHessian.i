// SWIG file FiniteDifferenceHessian.i
// @author dutka
// @date   2007-03-07 15:50:39 +0100 (mer. 07 mars 2007)

%{
#include "FiniteDifferenceHessian.hxx"
%}

%include FiniteDifferenceHessian.hxx
namespace OT { %extend FiniteDifferenceHessian { FiniteDifferenceHessian(const FiniteDifferenceHessian & other) { return new OT::FiniteDifferenceHessian(other); } } }
