// SWIG file LogNormal.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LogNormal.hxx"
%}

%include LogNormal.hxx
namespace OT { %extend LogNormal { LogNormal(const LogNormal & other) { return new OT::LogNormal(other); } } }
