// SWIG file Trapezoidal.i
// @author dutka
// @date   2007-03-07 15:50:39 +0100 (mer. 07 mars 2007)

%{
#include "Trapezoidal.hxx"
%}

%include Trapezoidal.hxx
namespace OT { %extend Trapezoidal { Trapezoidal(const Trapezoidal & other) { return new OT::Trapezoidal(other); } } }
