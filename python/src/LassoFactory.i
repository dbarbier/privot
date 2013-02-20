// SWIG file LassoFactory.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "LassoFactory.hxx"
%}

%include LassoFactory.hxx
namespace OT { %extend LassoFactory { LassoFactory(const LassoFactory & other) { return new OT::LassoFactory(other); } } }
