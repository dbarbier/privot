// SWIG file SharedPointer.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "SharedPointer.hxx"
%}

%include SharedPointer.hxx
namespace OT{ %extend SharedPointer { SharedPointer(const SharedPointer & other) { return new OT::SharedPointer(other); } } }
