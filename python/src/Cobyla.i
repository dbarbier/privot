// SWIG file Cobyla.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Cobyla.hxx"
%}

%include Cobyla.hxx
namespace OT{ %extend Cobyla { Cobyla(const Cobyla & other) { return new OT::Cobyla(other); } } }
