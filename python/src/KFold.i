// SWIG file KFold.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "KFold.hxx"
%}

%include KFold.hxx
namespace OT { %extend KFold { KFold(const KFold & other) { return new OT::KFold(other); } } }
