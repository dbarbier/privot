// SWIG file Null.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Null.hxx"
%}

%include Null.hxx
namespace OT{ %extend Null { Null(const Null & other) { return new OT::Null(other); } } }
