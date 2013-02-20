// SWIG file LogNormalFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LogNormalFactory.hxx"
%}

%include LogNormalFactory.hxx
namespace OT { %extend LogNormalFactory { LogNormalFactory(const LogNormalFactory & other) { return new OT::LogNormalFactory(other); } } }
