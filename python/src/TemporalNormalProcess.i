// SWIG file TemporalNormalProcess.i
// @author souchaud
// @date   2011-04-05 17:50:07 +0200 (mar. 05 avril 2011)

%{
#include "TemporalNormalProcess.hxx"
%}


%include TemporalNormalProcess.hxx
namespace OT { %extend TemporalNormalProcess { TemporalNormalProcess(const TemporalNormalProcess & other) { return new OT::TemporalNormalProcess(other); } } }
