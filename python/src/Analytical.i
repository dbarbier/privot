// SWIG file Analytical.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Analytical.hxx"
%}

%include Analytical.hxx
namespace OT{ %extend Analytical { Analytical(const Analytical & other) { return new OT::Analytical(other); } } }
