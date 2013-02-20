// SWIG file Study.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Study.hxx"
%}

%include Study.hxx
namespace OT{ %extend Study { Study(const Study & other) { return new OT::Study(other); } } }
