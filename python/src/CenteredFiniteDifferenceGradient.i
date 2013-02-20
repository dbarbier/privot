// SWIG file CenteredFiniteDifferenceGradient.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "CenteredFiniteDifferenceGradient.hxx"
%}

%include CenteredFiniteDifferenceGradient.hxx
namespace OT {  
%extend CenteredFiniteDifferenceGradient {

 CenteredFiniteDifferenceGradient(const CenteredFiniteDifferenceGradient & other)
 {
   return new OT::CenteredFiniteDifferenceGradient(other);
 }

 CenteredFiniteDifferenceGradient(const NumericalPoint & epsilon,
                                  const NumericalMathEvaluationImplementation & evaluation)
 {
   return new OT::CenteredFiniteDifferenceGradient(epsilon, evaluation.clone());
 }

 CenteredFiniteDifferenceGradient(const NumericalScalar epsilon,
                                  const NumericalMathEvaluationImplementation & evaluation)
 {
   return new OT::CenteredFiniteDifferenceGradient(epsilon, evaluation.clone());
 }


}

}
