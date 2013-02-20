// SWIG file LogUniformFactory.i
// @author dutka
// @date   2008-06-26 13:50:17 +0200 (jeu. 26 juin 2008)

%{
#include "LogUniformFactory.hxx"
%}

%include LogUniformFactory.hxx
namespace OT { %extend LogUniformFactory { LogUniformFactory(const LogUniformFactory & other) { return new OT::LogUniformFactory(other); } } }
