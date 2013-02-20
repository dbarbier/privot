// SWIG file TruncatedNormal.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "TruncatedNormal.hxx"
%}

%include TruncatedNormal.hxx
namespace OT { %extend TruncatedNormal { TruncatedNormal(const TruncatedNormal & other) { return new OT::TruncatedNormal(other); } } }
