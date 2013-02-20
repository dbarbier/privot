// SWIG file AdaptiveStrategy.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "AdaptiveStrategy.hxx"
%}

OTTypedInterfaceObjectHelper(AdaptiveStrategy)

%include AdaptiveStrategy.hxx
namespace OT{ %extend AdaptiveStrategy { AdaptiveStrategy(const AdaptiveStrategy & other) { return new OT::AdaptiveStrategy(other); } } }
