// SWIG file AdaptiveStrategyImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "AdaptiveStrategyImplementation.hxx"
%}

%include AdaptiveStrategyImplementation.hxx
namespace OT{ %extend AdaptiveStrategyImplementation { AdaptiveStrategyImplementation(const AdaptiveStrategyImplementation & other) { return new OT::AdaptiveStrategyImplementation(other); } } }
