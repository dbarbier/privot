// SWIG file Normal.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Normal.hxx"
%}

%include Normal.hxx
namespace OT { %extend Normal { Normal(const Normal & other) { return new OT::Normal(other); } } }
