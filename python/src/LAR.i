// SWIG file LAR.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "LAR.hxx"
%}

%include LAR.hxx
namespace OT { %extend LAR { LAR(const LAR & other) { return new OT::LAR(other); } } }
