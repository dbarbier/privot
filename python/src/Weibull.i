// SWIG file Weibull.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Weibull.hxx"
%}

%include Weibull.hxx
namespace OT { %extend Weibull { Weibull(const Weibull & other) { return new OT::Weibull(other); } } }
