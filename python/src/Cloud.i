// SWIG file Cloud.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Cloud.hxx"
%}

%include Cloud.hxx
namespace OT { %extend Cloud { Cloud(const Cloud & other) { return new OT::Cloud(other); } } }
