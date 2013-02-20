// SWIG file ARMAFactory.i
// @author schueller
// @date   2012-02-17 19:18:57 +0100 (Fri, 17 Feb 2012)

%{
#include "ARMAFactory.hxx"
%}

OTTypedInterfaceObjectHelper(ARMAFactory)

%include ARMAFactory.hxx
namespace OT { %extend ARMAFactory { ARMAFactory(const ARMAFactory & other) { return new OT::ARMAFactory(other); } } }

