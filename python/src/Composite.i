// SWIG file Composite.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Composite.hxx"
%}

%include Composite.hxx
namespace OT { %extend Composite { Composite(const Composite & other) { return new OT::Composite(other); } } }
