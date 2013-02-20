// SWIG file RestrictedGradientImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (lun. 02 janv. 2012)

%{
#include "RestrictedGradientImplementation.hxx"
%}

%include RestrictedGradientImplementation.hxx
namespace OT { %extend RestrictedGradientImplementation { RestrictedGradientImplementation(const RestrictedGradientImplementation & other) { return new OT::RestrictedGradientImplementation(other); } } }
