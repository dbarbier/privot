// SWIG file RandomDirection.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "RandomDirection.hxx"
%}

%include RandomDirection.hxx
namespace OT{ %extend RandomDirection { RandomDirection(const RandomDirection & other) { return new OT::RandomDirection(other); } } }
