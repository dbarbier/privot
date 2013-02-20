// SWIG file RestrictedHessianImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (lun. 02 janv. 2012)

%{
#include "RestrictedHessianImplementation.hxx"
%}

%include RestrictedHessianImplementation.hxx
namespace OT { %extend RestrictedHessianImplementation { RestrictedHessianImplementation(const RestrictedHessianImplementation & other) { return new OT::RestrictedHessianImplementation(other); } } }
