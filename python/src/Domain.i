// SWIG file Domain.i
// @author souchaud
// @date   2011-04-05 17:50:07 +0200 (mar. 05 avril 2011)

%{
#include "Domain.hxx"
%}

OTTypedInterfaceObjectHelper(Domain)

%include Domain.hxx

namespace OT { %extend Domain { Domain (const Domain & other) { return new OT::Domain(other); } } }
