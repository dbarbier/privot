// SWIG file Object.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Object.hxx"
%}

%include Object.hxx
namespace OT{ %extend Object { Object(const Object & other) { return new OT::Object(other); } } }
