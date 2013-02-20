// SWIG file Full.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Full.hxx"
%}

%include Full.hxx
namespace OT{ %extend Full { Full(const Full & other) { return new OT::Full(other); } } }
