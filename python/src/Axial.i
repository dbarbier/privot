// SWIG file Axial.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Axial.hxx"
%}

%include Axial.hxx
namespace OT { %extend Axial { Axial(const Axial & other) { return new OT::Axial(other); } } }
