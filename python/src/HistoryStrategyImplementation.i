// SWIG file HistoryStrategyImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "HistoryStrategyImplementation.hxx"
%}

%include HistoryStrategyImplementation.hxx
namespace OT{ %extend HistoryStrategyImplementation { HistoryStrategyImplementation(const HistoryStrategyImplementation & other) { return new OT::HistoryStrategyImplementation(other); } } }
