// SWIG file Curve.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Curve.hxx"
%}

%include Curve.hxx
namespace OT { %extend Curve { Curve(const Curve & other) { return new OT::Curve(other); } } }
