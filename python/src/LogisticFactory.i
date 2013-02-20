// SWIG file LogisticFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LogisticFactory.hxx"
%}

%include LogisticFactory.hxx
namespace OT { %extend LogisticFactory { LogisticFactory(const LogisticFactory & other) { return new OT::LogisticFactory(other); } } }
