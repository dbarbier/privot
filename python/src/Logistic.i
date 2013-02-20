// SWIG file Logistic.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Logistic.hxx"
%}

%include Logistic.hxx
namespace OT { %extend Logistic { Logistic(const Logistic & other) { return new OT::Logistic(other); } } }
