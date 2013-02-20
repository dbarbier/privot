// SWIG file Process.i
// @author souchaud
// @date   2011-04-05 17:50:07 +0200 (mar. 05 avril 2011)

%{
#include "Process.hxx"
%}

OTTypedInterfaceObjectHelper(Process)

%include Process.hxx
namespace OT { %extend Process { Process(const Process & other) { return new OT::Process(other); } } }

