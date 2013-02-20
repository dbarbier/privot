// SWIG file NonCenteredFiniteDifferenceGradient.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "NonCenteredFiniteDifferenceGradient.hxx"
%}

%include NonCenteredFiniteDifferenceGradient.hxx
namespace OT {  
%extend NonCenteredFiniteDifferenceGradient {

 NonCenteredFiniteDifferenceGradient(const NonCenteredFiniteDifferenceGradient & other)
 {
   return new OT::NonCenteredFiniteDifferenceGradient(other);
 }

 NonCenteredFiniteDifferenceGradient(const NumericalPoint & epsilon,
                                     const NumericalMathEvaluationImplementation & evaluation)
 {
   return new OT::NonCenteredFiniteDifferenceGradient(epsilon, evaluation.clone());
 }

 NonCenteredFiniteDifferenceGradient(const NumericalScalar epsilon,
                                     const NumericalMathEvaluationImplementation & evaluation)
 {
   return new OT::NonCenteredFiniteDifferenceGradient(epsilon, evaluation.clone());
 }

}

}
