// SWIG file Rayleigh.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Rayleigh.hxx"
%}

%include Rayleigh.hxx
namespace OT { %extend Rayleigh { Rayleigh(const Rayleigh & other) { return new OT::Rayleigh(other); } } }
