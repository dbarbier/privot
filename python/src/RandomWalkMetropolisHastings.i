// SWIG file RandomWalkMetropolisHastings.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "RandomWalkMetropolisHastings.hxx"
%}

%include RandomWalkMetropolisHastings.hxx
namespace OT { %extend RandomWalkMetropolisHastings { RandomWalkMetropolisHastings(const RandomWalkMetropolisHastings & other) { return new OT::RandomWalkMetropolisHastings(other); } } }
