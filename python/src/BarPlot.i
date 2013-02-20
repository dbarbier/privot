// SWIG file BarPlot.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "BarPlot.hxx"
%}

%include BarPlot.hxx
namespace OT { %extend BarPlot { BarPlot(const BarPlot & other) { return new OT::BarPlot(other); } } }
