// SWIG file Epanechnikov.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Epanechnikov.hxx"
%}

%include Epanechnikov.hxx
namespace OT { %extend Epanechnikov { Epanechnikov(const Epanechnikov & other) { return new OT::Epanechnikov(other); } } }
