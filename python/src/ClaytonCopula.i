// SWIG file ClaytonCopula.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ClaytonCopula.hxx"
%}

%include ClaytonCopula.hxx
namespace OT { %extend ClaytonCopula { ClaytonCopula(const ClaytonCopula & other) { return new OT::ClaytonCopula(other); } } }
