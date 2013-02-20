// SWIG file RootStrategyImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "RootStrategyImplementation.hxx"
%}

%include RootStrategyImplementation.hxx
namespace OT{ %extend RootStrategyImplementation { RootStrategyImplementation(const RootStrategyImplementation & other) { return new OT::RootStrategyImplementation(other); } } }
