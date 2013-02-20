// SWIG file Mesh.i
// @author schueller
// @date   2012-07-16 12:24:33 +0200 (Mon, 16 Jul 2012)

%{
#include "Mesh.hxx"
%}

%include Mesh.hxx

namespace OT { %extend Mesh { Mesh(const Mesh & other) { return new OT::Mesh(other); } } }
