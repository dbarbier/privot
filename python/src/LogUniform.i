// SWIG file LogUniform.i
// @author dutka
// @date   2007-03-07 15:50:39 +0100 (mer. 07 mars 2007)

%{
#include "LogUniform.hxx"
%}

%include LogUniform.hxx
namespace OT { %extend LogUniform { LogUniform(const LogUniform & other) { return new OT::LogUniform(other); } } }
