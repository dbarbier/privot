// SWIG file Threadable.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Threadable.hxx"
%}

%include Threadable.hxx
namespace OT{ %extend Threadable { Threadable(const Threadable & other) { return new OT::Threadable(other); } } }
