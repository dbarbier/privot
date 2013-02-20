// SWIG file InverseTrendTransform.i
// @author lebrun
// @date   2011-12-19 23:37:21 +0100 (lun. 19 déc. 2011)

%{
#include "InverseTrendTransform.hxx"
%}

%include InverseTrendTransform.hxx
namespace OT { %extend InverseTrendTransform { InverseTrendTransform(const InverseTrendTransform & other) { return new OT::InverseTrendTransform(other); } } }
