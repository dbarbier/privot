// SWIG file QuadraticTaylor.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "QuadraticTaylor.hxx"
%}

%include QuadraticTaylor.hxx
namespace OT { %extend QuadraticTaylor { QuadraticTaylor(const QuadraticTaylor & other) { return new OT::QuadraticTaylor(other); } } }
