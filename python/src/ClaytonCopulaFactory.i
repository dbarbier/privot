// SWIG file ClaytonCopulaFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ClaytonCopulaFactory.hxx"
%}

%include ClaytonCopulaFactory.hxx
namespace OT { %extend ClaytonCopulaFactory { ClaytonCopulaFactory(const ClaytonCopulaFactory & other) { return new OT::ClaytonCopulaFactory(other); } } }
