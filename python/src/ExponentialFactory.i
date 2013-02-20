// SWIG file ExponentialFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ExponentialFactory.hxx"
%}

%include ExponentialFactory.hxx
namespace OT { %extend ExponentialFactory { ExponentialFactory(const ExponentialFactory & other) { return new OT::ExponentialFactory(other); } } }
