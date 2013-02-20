// SWIG file Triangular.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Triangular.hxx"
%}

%include Triangular.hxx
namespace OT { %extend Triangular { Triangular(const Triangular & other) { return new OT::Triangular(other); } } }
