// SWIG file WeibullFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "WeibullFactory.hxx"
%}

%include WeibullFactory.hxx
namespace OT { %extend WeibullFactory { WeibullFactory(const WeibullFactory & other) { return new OT::WeibullFactory(other); } } }
