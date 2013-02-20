// SWIG file HyperbolicAnisotropicEnumerateFunction.i
// @author schueller
// @date   2010-12-15 18:09:41 +0100 (Wed, 15 Dec 2010)

%{
#include "HyperbolicAnisotropicEnumerateFunction.hxx"
%}

%include HyperbolicAnisotropicEnumerateFunction.hxx
namespace OT { %extend HyperbolicAnisotropicEnumerateFunction { HyperbolicAnisotropicEnumerateFunction(const HyperbolicAnisotropicEnumerateFunction & other) { return new OT::HyperbolicAnisotropicEnumerateFunction(other); } } }
