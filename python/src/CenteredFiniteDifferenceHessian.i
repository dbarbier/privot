// SWIG file CenteredFiniteDifferenceHessian.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "CenteredFiniteDifferenceHessian.hxx"
%}

%include CenteredFiniteDifferenceHessian.hxx
namespace OT {  
%extend CenteredFiniteDifferenceHessian {

 CenteredFiniteDifferenceHessian(const CenteredFiniteDifferenceHessian & other)
 {
   return new OT::CenteredFiniteDifferenceHessian(other);
 }

 CenteredFiniteDifferenceHessian(const NumericalPoint & epsilon,
                                 const NumericalMathEvaluationImplementation & evaluation)
 {
   return new OT::CenteredFiniteDifferenceHessian(epsilon, evaluation.clone());
 }

 CenteredFiniteDifferenceHessian(const NumericalScalar epsilon,
                                 const NumericalMathEvaluationImplementation & evaluation)
 {
   return new OT::CenteredFiniteDifferenceHessian(epsilon, evaluation.clone());
 }

}

}
