// SWIG file HistogramFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "HistogramFactory.hxx"
%}

%include HistogramFactory.hxx
namespace OT { %extend HistogramFactory { HistogramFactory(const HistogramFactory & other) { return new OT::HistogramFactory(other); } } }
