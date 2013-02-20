// SWIG file BoxCoxTransform.i
// @author schueller
// @date   2012-01-04 12:26:21 +0100 (Wed, 04 Jan 2012)

%{
#include "BoxCoxTransform.hxx"
%}

%include BoxCoxTransform.hxx
namespace OT { %extend BoxCoxTransform { BoxCoxTransform(const BoxCoxTransform & other) { return new OT::BoxCoxTransform(other); } } }
