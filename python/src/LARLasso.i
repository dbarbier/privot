// SWIG file LARLasso.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "LARLasso.hxx"
%}

%include LARLasso.hxx
namespace OT { %extend LARLasso { LARLasso(const LARLasso & other) { return new OT::LARLasso(other); } } }
