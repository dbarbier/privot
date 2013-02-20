// SWIG file Laplace.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Laplace.hxx"
%}

%include Laplace.hxx
namespace OT { %extend Laplace { Laplace(const Laplace & other) { return new OT::Laplace(other); } } }
