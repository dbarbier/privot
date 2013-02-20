// SWIG file BasisSequenceFactory.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "BasisSequenceFactory.hxx"
%}

OTTypedInterfaceObjectHelper(BasisSequenceFactory)

%include BasisSequenceFactory.hxx
namespace OT { %extend BasisSequenceFactory { BasisSequenceFactory(const BasisSequenceFactory & other) { return new OT::BasisSequenceFactory(other); } } }
