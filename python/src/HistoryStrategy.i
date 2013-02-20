// SWIG file HistoryStrategy.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "HistoryStrategy.hxx"
%}

OTTypedInterfaceObjectHelper(HistoryStrategy)

%include HistoryStrategy.hxx
namespace OT{ %extend HistoryStrategy { HistoryStrategy(const HistoryStrategy & other) { return new OT::HistoryStrategy(other); } } }
