// SWIG file CompositeProcess.i
// @author schueller
// @date   2012-01-04 12:26:21 +0100 (Wed, 04 Jan 2012)

%{
#include "CompositeProcess.hxx"
%}

%include CompositeProcess.hxx
namespace OT { %extend CompositeProcess { CompositeProcess(const CompositeProcess & other) { return new OT::CompositeProcess(other); } } }
