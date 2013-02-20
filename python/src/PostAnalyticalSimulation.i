// SWIG file PostAnalyticalSimulation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "PostAnalyticalSimulation.hxx"
%}

%include PostAnalyticalSimulation.hxx

namespace OT{ %extend PostAnalyticalSimulation { PostAnalyticalSimulation(const PostAnalyticalSimulation & other) { return new OT::PostAnalyticalSimulation(other); } } }
