// SWIG file TrendTransform.i
// @author schueller
// @date   2012-01-04 12:26:21 +0100 (Wed, 04 Jan 2012)

%{
#include "TrendTransform.hxx"
%}

%include TrendTransform.hxx
namespace OT { %extend TrendTransform { TrendTransform(const TrendTransform & other) { return new OT::TrendTransform(other); } } }
