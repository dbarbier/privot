// SWIG file Exponential.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Exponential.hxx"
%}

%include Exponential.hxx
namespace OT { %extend Exponential { Exponential(const Exponential & other) { return new OT::Exponential(other); } } }
