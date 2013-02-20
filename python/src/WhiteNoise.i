// SWIG file WhiteNoise.i
// @author dutka
// @date   2007-03-07 15:50:39 +0100 (mer. 07 mars 2007)

%{
#include "WhiteNoise.hxx"
%}

%include WhiteNoise.hxx
namespace OT { %extend WhiteNoise { WhiteNoise(const WhiteNoise & other) { return new OT::WhiteNoise(other); } } }
