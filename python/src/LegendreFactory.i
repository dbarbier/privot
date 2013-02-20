// SWIG file LegendreFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LegendreFactory.hxx"
%}

%include LegendreFactory.hxx
namespace OT { %extend LegendreFactory { LegendreFactory(const LegendreFactory & other) { return new OT::LegendreFactory(other); } } }
