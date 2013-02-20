// SWIG file RootStrategy.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "RootStrategy.hxx"
%}

OTTypedInterfaceObjectHelper(RootStrategy)

%include RootStrategy.hxx
namespace OT{ %extend RootStrategy { RootStrategy(const RootStrategy & other) { return new OT::RootStrategy(other); } } }
