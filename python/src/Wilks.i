// SWIG file Wilks.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Wilks.hxx"
%}

%include Wilks.hxx
namespace OT { %extend Wilks { Wilks(const Wilks & other) { return new OT::Wilks(other); } } }
