// SWIG file InverseNormal.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "InverseNormal.hxx"
%}

%include InverseNormal.hxx
namespace OT { %extend InverseNormal { InverseNormal(const InverseNormal & other) { return new OT::InverseNormal(other); } } }
