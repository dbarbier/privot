// SWIG file OrthogonalDirection.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "OrthogonalDirection.hxx"
%}

%include OrthogonalDirection.hxx
namespace OT{ %extend OrthogonalDirection { OrthogonalDirection(const OrthogonalDirection & other) { return new OT::OrthogonalDirection(other); } } }
