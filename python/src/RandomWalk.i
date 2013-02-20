// SWIG file RandomWalk.i
// @author dutka
// @date   2007-03-07 15:50:39 +0100 (mer. 07 mars 2007)

%{
#include "RandomWalk.hxx"
%}

%include RandomWalk.hxx
namespace OT { %extend RandomWalk { RandomWalk(const RandomWalk & other) { return new OT::RandomWalk(other); } } }
