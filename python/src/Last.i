// SWIG file Last.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Last.hxx"
%}

%include Last.hxx
namespace OT{ %extend Last { Last(const Last & other) { return new OT::Last(other); } } }
